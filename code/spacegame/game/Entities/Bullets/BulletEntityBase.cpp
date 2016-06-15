#include <game/Entities/Bullets/BulletEntityBase.hpp>

#include <game/GameGlobals.hpp>
#include <game/Entities/Actors/PlayerEntity.hpp>

////////////////////////////////////////////////////
BulletEntityBase::BulletEntityBase() :
	m_damage	(0.f),
	m_speed		(10.f),
	m_shooter	(NULL),
	m_angle		(0.f)
{
	setBeginContactCallback(&BulletEntityBase::begincontact);

	setCollisionFilterCallback(&BulletEntityBase::collisionCallback);
}

////////////////////////////////////////////////////
BulletEntityBase::~BulletEntityBase()
{
	
}

////////////////////////////////////////////////////
float BulletEntityBase::getDamage()
{
	if(m_shooter == GameGlobals::get()->player)
	{
		return m_damage * dynamic_cast<PlayerEntity*>(m_shooter)->getDamageModifier();
	}

	return m_damage;
}

////////////////////////////////////////////////////
void BulletEntityBase::shoot(sf::Vector2f position, float angle, float offset, float speed)
{
	m_direction = sf::Vector2f(
		std::cos(angle),
		std::sin(angle)
	);

	position += m_direction * offset;

	//getComponent<sz::Transform>()->setPosition(position);

	sz::Physics* physics = getComponent<sz::Physics>();

	m_speed = speed;
	sf::Vector2f velocity = m_direction * m_speed;

	physics->setPosition(position);
	physics->setLinearVelocity(velocity);
	
	m_angle = angle;

	getComponent<sz::Transform>()->setRotation(angle);
}

////////////////////////////////////////////////////
void BulletEntityBase::update()
{
	derivedUpdate();

	m_direction = sf::Vector2f(
		std::cos(m_angle),
		std::sin(m_angle)
	);

	sf::Vector2f velocity = m_direction * m_speed;
	getComponent<sz::Physics>()->setLinearVelocity(velocity);

	//float angle = sz::toDegrees(sz::getAngle(m_direction));
	//getComponent<sz::Transform>()->setRotation(angle);
}

////////////////////////////////////////////////////
bool BulletEntityBase::collisionCallback(sz::PhysicsContact& contact)
{
	bool collide = m_shooter != contact.entityB && dynamic_cast<BulletEntityBase*>(contact.entityB) == NULL;
	return collide;
}

////////////////////////////////////////////////////
void BulletEntityBase::begincontact(sz::PhysicsContact& contact)
{
	if(onImpact(contact.entityB))
	{
		destroyEntity();
	}
}