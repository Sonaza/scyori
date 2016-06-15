#include <szen/Game/World.hpp>

#include <szen/Game/Entity.hpp>
#include <szen/Game/Component.hpp>

#include <szen/System/Window.hpp>
#include <szen/System/InputManager.hpp>

#include <szen/System/SpriteBatch.hpp>

#include <szen/System/Scene.hpp>

//#include <szen/System/ThreadPool.hpp>

#include <algorithm>

using namespace sz;

EntityList	World::m_entities;
EntityList	World::m_pendingEntities;

float		World::m_worldScale		= 30.f;

b2World*	World::m_world			= NULL;
float		World::m_physicsStep	= 1.f / 60.f;
sf::Int32	World::m_velIterations	= 7;
sf::Int32	World::m_posIterations	= 7;

WorldContactListener	m_contactListener;
WorldContactFilter		m_contactFilter;

bool		World::m_pendingEntitySort = false;

namespace
{

	//std::unique_ptr<ThreadPool>		m_threadPool;

	float targetStep;
	float currentStep;

	sf::RenderWindow* p_renderWindow = NULL;
}

////////////////////////////////////////////////////
void WorldContactListener::BeginContact(b2Contact* bcontact)
{
	Entity* entityA = static_cast<Entity*>(bcontact->GetFixtureA()->GetBody()->GetUserData());
	Entity* entityB = static_cast<Entity*>(bcontact->GetFixtureB()->GetBody()->GetUserData());

	PhysicsContact contact(entityA, entityB, bcontact);

	if(entityA && entityA->m_beginContactCallback)
	{
		entityA->m_beginContactCallback(contact);
	}

	// Swap entities so that A is self
	std::swap(contact.entityA, contact.entityB);

	if(entityB && entityB->m_beginContactCallback)
	{
		entityB->m_beginContactCallback(contact);
	}
}

////////////////////////////////////////////////////
void WorldContactListener::EndContact(b2Contact* bcontact)
{
	Entity* entityA = static_cast<Entity*>(bcontact->GetFixtureA()->GetBody()->GetUserData());
	Entity* entityB = static_cast<Entity*>(bcontact->GetFixtureB()->GetBody()->GetUserData());

	PhysicsContact contact(entityA, entityB, bcontact);

	if(entityA && entityA->m_endContactCallback)
	{
		entityA->m_endContactCallback(contact);
	}

	// Swap entities so that A is self
	std::swap(contact.entityA, contact.entityB);

	if(entityB && entityB->m_endContactCallback)
	{
		entityB->m_endContactCallback(contact);
	}
}

////////////////////////////////////////////////////
bool WorldContactFilter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
	Entity* entityA = static_cast<Entity*>(fixtureA->GetBody()->GetUserData());
	Entity* entityB = static_cast<Entity*>(fixtureB->GetBody()->GetUserData());

	PhysicsContact contact(entityA, entityB, fixtureA, fixtureB);

	bool result = true;

	if(entityA && entityA->m_collisionFilterCallback)
	{
		result = result && entityA->m_collisionFilterCallback(contact);
	}
	else
	{
		result = result
			&& (entityA->m_collisionfilter.maskBits & entityB->m_collisionfilter.categoryBits) != 0;
	}

	if(result)
	{
		// Swap entities so that A is self
		std::swap(contact.entityA, contact.entityB);
		std::swap(contact.fixtureA, contact.fixtureB);

		if(entityB && entityB->m_collisionFilterCallback)
		{
			result = result && entityB->m_collisionFilterCallback(contact);
		}
		else
		{
			result = result
				&& (entityA->m_collisionfilter.categoryBits & entityB->m_collisionfilter.maskBits) != 0;
		}
	}

	return result;
}

////////////////////////////////////////////////////
void World::init(float gx, float gy)
{
	init(sf::Vector2f(gx, gy));
}

////////////////////////////////////////////////////
void World::init(const sf::Vector2f &gravity)
{
	assert(m_world == NULL && "World must be uninitialized");

	// Create the box2d world
	m_world = new(std::nothrow) b2World(b2Vec2(gravity.x, gravity.y));
	assert(m_world != NULL && "Box2D physics world allocation failed");

	m_world->SetAllowSleeping(true);

	// Hook contact listener and filter
	m_world->SetContactListener(&m_contactListener);
	m_world->SetContactFilter(&m_contactFilter);

	m_entities.reserve(1000);

	p_renderWindow = Window::getRenderWindow();

	//m_threadPool.reset(new ThreadPool(1));
}

////////////////////////////////////////////////////
void World::uninit()
{
	clearEntities();

	if(m_world != NULL)
	{
		delete m_world;
		m_world = NULL;
	}
}

////////////////////////////////////////////////////
void World::setProperties(const sf::Vector2f &gravity)
{
	m_world->SetGravity(b2Vec2(gravity.x, gravity.y));
}

////////////////////////////////////////////////////
void World::setProperties(float timeStep, sf::Int32 velocityIterations, sf::Int32 positionIterations)
{
	m_physicsStep = timeStep;

	if(velocityIterations > 0) m_velIterations = velocityIterations;
	if(positionIterations > 0) m_posIterations = positionIterations;
}

////////////////////////////////////////////////////
float World::getWorldScale()
{
	return m_worldScale;
}

////////////////////////////////////////////////////
b2World* World::getBox2DWorld()
{
	return m_world;
}

////////////////////////////////////////////////////
size_t World::getEntityCount()
{
	return m_entities.size();
}

////////////////////////////////////////////////////
void World::clearEntities()
{
	addPendingEntities(false);
	
	for(EntityList::iterator it = m_entities.begin();
		it != m_entities.end(); ++it)
	{
		Entity* e = *it;
		delete e;
	}

	m_entities.clear();
}

////////////////////////////////////////////////////
void World::updateWorld()
{
	if(Scene::isPaused()) return;

	if(m_world)
	{
		m_world->Step(m_physicsStep, m_velIterations, m_posIterations);
	}

	addPendingEntities();

	Entity* entity = NULL;
	for(EntityList::iterator it = m_entities.begin(); it != m_entities.end();)
	{
		entity = *it;

		if(!entity->isDestroyed())
		{
			entity->updateComponents();

			//m_threadPool->enqueue(&Entity::updateComponents, entity);

			++it;
		}
		else
		{
			delete *it;
			it = m_entities.erase(it);
		}
	}
}

////////////////////////////////////////////////////
std::vector<Entity*> World::getChildren(Entity* e)
{
	std::vector<Entity*> result;

	EntityList combined(m_entities.begin(), m_entities.end());
	if(!m_pendingEntities.empty())
		combined.insert(combined.end(), m_pendingEntities.begin(), m_pendingEntities.end());

	for(auto it = combined.begin(); it != combined.end(); ++it)
	{
		if((*it)->getParent() == e)
		{
			result.push_back(*it);
		}
	}

	return result;
}

////////////////////////////////////////////////////
void World::destroyChildren(Entity* e)
{
	auto children = getChildren(e);

	if(children.empty()) return;

	for(auto it = children.begin(); it != children.end(); ++it)
	{
		(*it)->destroyEntity();
	}
}

////////////////////////////////////////////////////
void World::renderWorld()
{
	if(m_pendingEntitySort)
	{
		sortEntities();
	}

	SpriteBatch::clear();

	for(EntityList::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		if((*it)->getLayer() < EntityLayer::Interface)
			p_renderWindow->draw(**it);
	}

	SpriteBatch::draw(*p_renderWindow);
}

////////////////////////////////////////////////////
void World::renderInterface()
{
	if(m_pendingEntitySort)
	{
		sortEntities();
	}

	SpriteBatch::clear();

	for(EntityList::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		if((*it)->getLayer() >= EntityLayer::Interface)
			p_renderWindow->draw(**it);
	}

	SpriteBatch::draw(*p_renderWindow);
}

////////////////////////////////////////////////////
void World::sortEntities()
{
	if(!m_pendingEntitySort) return;
	m_pendingEntitySort = false;

	std::sort(m_entities.begin(), m_entities.end(), Entity::layerSorter);
}

////////////////////////////////////////////////////
void World::addPendingEntities(const bool update)
{
	// Add pending entities
	if(!m_pendingEntities.empty())
	{
		if(update)
		{
			std::for_each(m_pendingEntities.begin(), m_pendingEntities.end(),
				[](Entity* e)
				{
					e->updateComponents();
					e->update();
				}
			);
		}

		m_entities.insert(m_entities.end(), m_pendingEntities.begin(), m_pendingEntities.end());
		m_pendingEntities.clear();

		m_pendingEntitySort = true;
	}
}

////////////////////////////////////////////////////
Entity* World::findWithTag(const std::string& tag)
{
	if(m_entities.empty() && m_pendingEntities.empty()) return NULL;

	EntityList combined(m_entities.begin(), m_entities.end());
	if(!m_pendingEntities.empty())
		combined.insert(combined.end(), m_pendingEntities.begin(), m_pendingEntities.end());

	EntityList::iterator it = std::find_if(
		combined.begin(),
		combined.end(), 
		[&](Entity* a) { return a->getTag() == tag; }
	);

	return it != combined.end() ? *it : NULL;
}

////////////////////////////////////////////////////
bool World::exists(Entity* entity)
{
	if(m_entities.empty() && m_pendingEntities.empty()) return false;

	EntityList combined(m_entities.begin(), m_entities.end());
	if(!m_pendingEntities.empty())
		combined.insert(combined.end(), m_pendingEntities.begin(), m_pendingEntities.end());

	return std::find(combined.begin(), combined.end(), entity) != combined.end();
}