#include <szen/Game/Components/Physics.hpp>

#include <szen/Game/Entity.hpp>
#include <szen/Game/Components/Transform.hpp>

#include <szen/Game/World.hpp>
#include <szen/System/ErrorStream.hpp>

#include <szen/System/Utility.hpp>

using namespace sz;

////////////////////////////////////////////////////
Physics::Physics() :
	m_body(NULL),
	p_world(NULL),
	m_worldScale(1.f),
	m_speedLimit(0.f)
{
	m_componentOrder = 0;
}

////////////////////////////////////////////////////
Physics::~Physics()
{
	if(p_world && m_body)
	{
		p_world->DestroyBody(m_body);
	}
}

////////////////////////////////////////////////////
void Physics::attached()
{
	assert(m_entity->hasComponent<Transform>());

	p_world = World::getBox2DWorld();
	m_worldScale = World::getWorldScale();
}

////////////////////////////////////////////////////
void Physics::parsePrefab(json::Value& val)
{
	const std::string bodyshape = val["shape"].asString();
	const std::string bodytype = val["bodytype"].asString();

	const bool isBullet = val.get("bullet", 0).asBool();

	BodyType type;
	if(bodytype == "dynamic")	type = Dynamic; else
	if(bodytype == "static")	type = Static; else
	if(bodytype == "kinematic")	type = Kinematic;
	else szerr << "Unsupported body type: " << bodytype << ErrorStream::error;

	if(bodyshape == "box")
	{
		createBox(
			static_cast<float>(val["size"][0U].asDouble()),
			static_cast<float>(val["size"][1U].asDouble()),
			type, isBullet
		);
	}
	else if(bodyshape == "circle")
	{
		createCircle(
			static_cast<float>(val["radius"].asDouble()),
			type, isBullet
		);
	}
	else if(bodyshape == "polygon")
	{
		std::vector<sf::Vector2f> vertices;

		sf::Uint32 count = val["vertices"].size();
		for(sf::Uint32 i=0; i < count; i += 2)
		{
			vertices.push_back(sf::Vector2f(
				static_cast<float>(val["vertices"][i+0].asDouble()),
				static_cast<float>(val["vertices"][i+1].asDouble())
			));
		}

		createPolygon(vertices, type, isBullet);
	}
	else
	{
		szerr << "Unsupported body shape: " << bodyshape << ErrorStream::error;
	}

	finalizeBody(
		static_cast<float>(val["friction"].asDouble()),
		static_cast<float>(val["restitution"].asDouble()),
		static_cast<float>(val["density"].asDouble())
	);

	m_body->SetGravityScale(
		static_cast<float>(val.get("gravityscale", 1).asDouble())
	);

	setSpeedLimit(
		static_cast<float>(val.get("speedlimit", 0.f).asDouble())
	);
}

////////////////////////////////////////////////////
void Physics::_createBody(BodyType type, const bool isBullet)
{	
	assert(m_entity && "Component has not been attached");

	m_bodyDef.type = static_cast<b2BodyType>(type);

	Transform* transform = m_entity->getComponent<Transform>();
	sf::Vector2f position = transform->getPosition() / m_worldScale;

	m_bodyDef.position.Set(position.x, position.y);
	m_bodyDef.angle = transform->getRotation() * b2_pi / 180.f;
	
	m_bodyDef.bullet = isBullet;

	// Set user data to point back to our parent entity
	// in case we need to do something directly with it
	m_bodyDef.userData = m_entity;

	m_body = p_world->CreateBody(&m_bodyDef);
}

////////////////////////////////////////////////////
void Physics::createBox(sf::Vector2f size, BodyType type, const bool isBullet)
{
	_createBody(type, isBullet);
	
	Transform* transform = m_entity->getComponent<Transform>();
	sf::Vector2f scale = transform->getScale();

	// Create box shape scaled according to the world scale
	m_bodyShape.polygon.SetAsBox(
		(size.x - 2.f) / m_worldScale * scale.x * 0.5f,
		(size.y - 2.f) / m_worldScale * scale.y * 0.5f);

	m_fixtureDef.shape = &m_bodyShape.polygon;
}

////////////////////////////////////////////////////
void Physics::createBox(float sx, float sy, BodyType type, const bool isBullet)
{
	createBox(sf::Vector2f(sx, sy), type);
}

////////////////////////////////////////////////////
void Physics::createCircle(float radius, BodyType type, const bool isBullet)
{
	_createBody(type, isBullet);

	Transform* transform = m_entity->getComponent<Transform>();
	sf::Vector2f scale = transform->getScale();

	// Set circle shape radius and relative position to body
	m_bodyShape.circle.m_radius = radius / m_worldScale * scale.x;
	m_bodyShape.circle.m_p.Set(0.f, 0.f);

	m_fixtureDef.shape = &m_bodyShape.circle;
}

////////////////////////////////////////////////////
void Physics::createPolygon(std::vector<sf::Vector2f> vertices, BodyType type, const bool isBullet)
{
	_createBody(type, isBullet);

	Transform* transform = m_entity->getComponent<Transform>();
	sf::Vector2f scale = transform->getScale();

	// Create a temporary array for vertices and copy them over
	std::vector<b2Vec2> temp;
	std::for_each(
		vertices.begin(), vertices.end(),
		[&temp, &scale, this](sf::Vector2f p)
		{
			temp.push_back(b2Vec2(p.x / m_worldScale * scale.x, p.y / m_worldScale * scale.y));
		});

	// Set polygon vertices
	m_bodyShape.polygon.Set(&temp[0], temp.size());

	m_fixtureDef.shape = &m_bodyShape.polygon;
}

////////////////////////////////////////////////////
void Physics::finalizeBody(float friction, float restitution, float density)
{
	m_fixtureDef.friction		= friction;
	m_fixtureDef.restitution	= restitution;
	m_fixtureDef.density		= density;

	// Apply the fixture to the body and we're golden
	m_body->CreateFixture(&m_fixtureDef);
}

////////////////////////////////////////////////////
void Physics::update()
{
	Transform* t = m_entity->getComponent<Transform>();

	b2Vec2 temp = m_body->GetPosition();

	/*if(t->m_positionUpdated)
	{
		sf::Vector2f pos = t->getPosition();
		pos.y *= -1.f;
		setPosition(pos);
		t->m_positionUpdated = false;
	}
	else
	{*/
		t->setPosition(temp.x * m_worldScale, -temp.y * m_worldScale);
	//}
	
	t->setRotation(m_body->GetAngle() * 180.f / b2_pi);

	if(m_speedLimit > 0.f)
	{
		b2Vec2 vel = m_body->GetLinearVelocity();
		float speed = vel.Length();
		if(speed > m_speedLimit)
		{
			m_body->SetLinearVelocity((m_speedLimit / speed) * vel);
		}
	}
}

////////////////////////////////////////////////////
void Physics::setPosition(sf::Vector2f v)
{
	b2Vec2 vec(v.x / m_worldScale, v.y / m_worldScale);
	m_body->SetTransform(vec, m_body->GetAngle());
}

////////////////////////////////////////////////////
void Physics::setRotation(float a)
{
	m_body->SetTransform(m_body->GetPosition(), a * b2_pi / 180.f);
}

////////////////////////////////////////////////////
sf::Vector2f Physics::getLinearVelocity()
{
	b2Vec2 vel = m_body->GetLinearVelocity();
	return sf::Vector2f(vel.y, vel.x);
}

////////////////////////////////////////////////////
void Physics::setLinearVelocity(sf::Vector2f v)
{
	m_body->SetLinearVelocity(b2Vec2(
		v.x,
		v.y
	));
}

////////////////////////////////////////////////////
float Physics::getAngularVelocity()
{
	return m_body->GetAngularVelocity();
}

////////////////////////////////////////////////////
void Physics::setAngularVelocity(float w)
{
	m_body->SetAngularVelocity(w);
}

////////////////////////////////////////////////////
void Physics::applyTorque(float torque)
{
	m_body->ApplyTorque(torque);
}

////////////////////////////////////////////////////
float Physics::getLinearDamping()
{
	return m_body->GetLinearDamping();
}

////////////////////////////////////////////////////
void Physics::setLinearDamping(float v)
{
	m_body->SetLinearDamping(v);
}

////////////////////////////////////////////////////
void Physics::setSpeedLimit(float limit)
{
	if(limit > 0.f)
	{
		m_speedLimit = limit / m_worldScale;
		m_speedLimit *= m_speedLimit;
	}
	else
	{
		m_speedLimit = -1.f;
	}
}

////////////////////////////////////////////////////
void Physics::linearImpulse(float angleRadians, float force)
{
	b2Vec2 direction(std::cos(angleRadians), std::sin(angleRadians));
	m_body->ApplyLinearImpulse(force * direction, m_body->GetWorldCenter());
}

////////////////////////////////////////////////////
void Physics::linearImpulse(sf::Vector2f& force)
{
	m_body->ApplyLinearImpulse(b2Vec2(force.x, force.y), m_body->GetWorldCenter());
}

////////////////////////////////////////////////////
void Physics::accelerate(float angleRadians, float amount)
{
	b2Vec2 direction(std::cos(angleRadians), std::sin(angleRadians));
	m_body->SetLinearVelocity(m_body->GetLinearVelocity() + amount * direction);
}

////////////////////////////////////////////////////
void Physics::accelerate(sf::Vector2f& amount)
{
	m_body->SetLinearVelocity(m_body->GetLinearVelocity() + b2Vec2(amount.x, amount.y));
}

////////////////////////////////////////////////////
void Physics::brake(float intensity)
{
	m_body->SetLinearVelocity((1.f - intensity) * m_body->GetLinearVelocity());
}

////////////////////////////////////////////////////
namespace
{

	class QueryCallback : public b2QueryCallback
	{
	public:
		////////////////////////////////////////////////////
		QueryCallback(sf::Vector2f& point, float radius) :
			m_worldscale(World::getWorldScale()),
			radius(radius * radius / m_worldscale),
			m_body(NULL),
			m_position(point.x / m_worldscale, point.y / m_worldscale),
			doDistanceCheck(radius > 0.f)
		{}

		////////////////////////////////////////////////////
		QueryCallback(b2Body* body, float radius) :
			m_worldscale(World::getWorldScale()),
			radius((radius / m_worldscale) * (radius / m_worldscale)),
			m_body(body),
			m_position(body->GetPosition()),
			doDistanceCheck(radius > 0.f)
		{}

		std::vector<Entity*>	foundEntities;

		////////////////////////////////////////////////////
		bool ReportFixture(b2Fixture* fixture)
		{
			b2Body* body = fixture->GetBody();
			b2Vec2 position = body->GetPosition();

			Entity* entity = static_cast<Entity*>(body->GetUserData());

			if(m_body != body && entity)
			{
				auto it = std::find(foundEntities.begin(), foundEntities.end(), entity);
				if(it == foundEntities.end())
				{
					if(!doDistanceCheck)
					{
						foundEntities.push_back(entity);
					}
					else
					{
						float32 dist = (position - m_position).Length();

						if(dist <= radius)
						{
							foundEntities.push_back(entity);
							distances.insert(std::make_pair(entity, dist));
						}
					}
				}
			}

			return true;
		}

		////////////////////////////////////////////////////
		void sortClosest()
		{
			if(distances.empty()) return;

			std::sort(foundEntities.begin(), foundEntities.end(),
				[this](Entity* a, Entity* b)
				{
					return distances[a] < distances[b];
				}
			);
		}

		////////////////////////////////////////////////////
		void cut(unsigned int limit)
		{
			if(foundEntities.empty()) return;

			limit = std::min(limit, foundEntities.size());
			foundEntities.resize(limit);
		}

	//protected:

		float			m_worldscale;

		sf::Vector2f	point;
		float			radius;

		b2Body*			m_body;
		b2Vec2			m_position;

		std::map<Entity*, float>	distances;

		const bool		doDistanceCheck;

	};

}

////////////////////////////////////////////////////
std::vector<Entity*> Physics::queryRadius(float radius, uint32 limit)
{
	QueryCallback queryCallback(m_body, radius);

	b2Vec2 point = m_body->GetPosition();

	radius /= m_worldScale;

	b2AABB aabb;
	aabb.lowerBound.Set(point.x - radius, point.y - radius);
	aabb.upperBound.Set(point.x + radius, point.y + radius);

	p_world->QueryAABB(&queryCallback, aabb);

	if(limit > 0)
	{
		queryCallback.sortClosest();
		queryCallback.cut(limit);
	}

	return queryCallback.foundEntities;
}

////////////////////////////////////////////////////
std::vector<Entity*> Physics::queryAABB(sf::Vector2f& point, sf::Vector2f& rect, unsigned int limit)
{
	QueryCallback queryCallback(point, 0.f);

	point /= World::getWorldScale();
	rect = rect / World::getWorldScale();

	b2AABB aabb;
	aabb.lowerBound.Set(point.x - rect.x, point.y - rect.y);
	aabb.upperBound.Set(point.x + rect.x, point.y + rect.y);

	World::getBox2DWorld()->QueryAABB(&queryCallback, aabb);

	if(limit > 0)
	{
		queryCallback.sortClosest();
		queryCallback.cut(limit);
	}

	return queryCallback.foundEntities;
}

////////////////////////////////////////////////////
std::vector<Entity*> Physics::queryRadius(sf::Vector2f& point, float radius, unsigned int limit)
{
	QueryCallback queryCallback(point, radius);

	point /= World::getWorldScale();
	radius /= World::getWorldScale();

	b2AABB aabb;
	aabb.lowerBound.Set(point.x - radius, -point.y - radius);
	aabb.upperBound.Set(point.x + radius, -point.y + radius);

	World::getBox2DWorld()->QueryAABB(&queryCallback, aabb);

	if(limit > 0)
	{
		queryCallback.sortClosest();
		queryCallback.cut(limit);
	}

	return queryCallback.foundEntities;
}