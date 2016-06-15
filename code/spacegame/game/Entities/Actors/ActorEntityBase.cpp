#include <game/Entities/Actors/ActorEntityBase.hpp>
#include <game/Entities/Bullets/BulletEntityBase.hpp>

////////////////////////////////////////////////////
ActorEntityBase::ActorEntityBase() :
	m_type(Unknown)
{
	p_gameGlobals = GameGlobals::get();
}

////////////////////////////////////////////////////
ActorEntityBase::~ActorEntityBase()
{
	
}

////////////////////////////////////////////////////
void ActorEntityBase::shoot(BulletEntityBase* bullet, float angle, float offset, float velocity)
{
	sf::Vector2f pos = getComponent<sz::Transform>()->getPosition();

	bullet->m_shooter = this;
	bullet->shoot(pos, angle, offset, velocity);
}