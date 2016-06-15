#include <szen/Game/Entity.hpp>
#include <szen/Game/Component.hpp>
#include <iostream>

#include <szen/System/ErrorStream.hpp>
#include <szen/Game/Component.hpp>

#include <szen/Game/World.hpp>

#include <szen/Game/PrefabLoader.hpp>

using namespace sz;

////////////////////////////////////////////////////
Entity::Entity() :
	m_isEnabled		(true),
	m_isDestroyed	(false),
	m_entityLayer	(EntityLayer::Foreground),
	m_parentEntity	(NULL),
	m_isParent		(false),
	m_tag			("")
{
	
}

////////////////////////////////////////////////////
Entity::~Entity()
{
	for(ComponentList::iterator it = m_components.begin();
		it != m_components.end(); ++it)
	{
		delete it->second;
	}

	m_components.clear();
}

////////////////////////////////////////////////////
bool Entity::loadFromPrefab(const std::string& path)
{
	return PrefabLoader::loadPrefab(path, this);
}

////////////////////////////////////////////////////
bool Entity::hasComponents() const
{
	return !m_components.empty();
}

#include <szen/Game/Components/TextRenderer.hpp>

////////////////////////////////////////////////////
void Entity::sortComponents()
{
	m_pendingSort = false;

	m_sortedComponents.clear();

	for(ComponentList::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if(dynamic_cast<TextRenderer*>(it->second) != NULL) continue;

		m_sortedComponents.push_back(it->second);
	}

	std::sort(m_sortedComponents.begin(), m_sortedComponents.end(),
		[](Component* a, Component* b) { return a->m_componentOrder < b->m_componentOrder; }
	);
}

////////////////////////////////////////////////////
void Entity::updateComponents()
{
	if(!m_isEnabled) return;

	updateClocks();

	if(m_pendingSort)
	{
		sortComponents();
	}

	//for(ComponentList::iterator it = m_components.begin(); it != m_components.end(); ++it)
	for(SortedComponentList::iterator it = m_sortedComponents.begin(); it != m_sortedComponents.end(); ++it)
	{
		//Component* c = it->second;
		//it->second->update();
		//if(dynamic_cast<TextRenderer*>(*it) != NULL) continue;
		(*it)->update();
	}

	update();
	internalUpdate();

	auto tr = getComponent<TextRenderer>();
	if(tr != NULL) tr->update();
}

////////////////////////////////////////////////////
void Entity::setBeginContactCallback(void (*f)(PhysicsContact&))
{
	m_beginContactCallback = std::bind(f, std::placeholders::_1);
}

////////////////////////////////////////////////////
void Entity::setEndContactCallback(void (*f)(PhysicsContact&))
{
	m_endContactCallback = std::bind(f, std::placeholders::_1);
}

////////////////////////////////////////////////////
void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(m_isEnabled && !m_drawCallbacks.empty())
	{
		for(std::vector<DrawFunction>::const_iterator it = m_drawCallbacks.begin();
			it != m_drawCallbacks.end(); ++it)
		{
			(*it)(target);
		}
		//m_drawCallback(target);
	}
}

////////////////////////////////////////////////////
void Entity::setEnabled(bool enabled)
{
	//if(hasParent() && !m_parentEntity->isEnabled());
	m_isEnabled = enabled;

	if(m_isParent)
	{
		auto children = World::getChildren(this);
		for(auto it = children.begin(); it != children.end(); ++it)
		{
			(*it)->setEnabled(enabled);
		}
	}
}

////////////////////////////////////////////////////
bool Entity::isEnabled() const
{
	return m_isEnabled;
}

////////////////////////////////////////////////////
void Entity::destroyEntity()
{
	m_isDestroyed = true;
	m_isEnabled = false;

	if(m_isParent) World::destroyChildren(this);
}

////////////////////////////////////////////////////
bool Entity::isDestroyed() const
{
	return m_isDestroyed;
}

////////////////////////////////////////////////////
void Entity::setTag(const std::string &tag)
{
	m_tag = tag;
}

////////////////////////////////////////////////////
std::string Entity::getTag() const
{
	return m_tag;
}

////////////////////////////////////////////////////
void Entity::setLayer(const EntityLayer::Level layerlevel, const EntityLayer::Id layerid)
{
	m_entityLayer = layerlevel + layerid;
	World::m_pendingEntitySort = true;
}

////////////////////////////////////////////////////
EntityLayer::Id Entity::getLayer() const
{
	return m_entityLayer;
}

////////////////////////////////////////////////////
bool Entity::layerSorter(Entity* a, Entity* b)
{
	return a->m_entityLayer < b->m_entityLayer;
}

////////////////////////////////////////////////////
void Entity::updateClocks()
{
	Time.deltaTime	= m_deltaTimeClock.restart();
	Time.delta		= Time.deltaTime.asSeconds();

	Time.totalTime	= m_totalTimeClock.getElapsedTime();
	Time.total		= Time.totalTime.asSeconds();
}

////////////////////////////////////////////////////
void Entity::setParent(Entity* parent)
{
	m_parentEntity = parent;
	parent->m_isParent = true;
}

////////////////////////////////////////////////////
Entity* Entity::getParent() const
{
	return m_parentEntity;
}

////////////////////////////////////////////////////
bool Entity::hasParent()
{
	return m_parentEntity != NULL;
}