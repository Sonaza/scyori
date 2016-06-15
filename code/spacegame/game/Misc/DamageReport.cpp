#include <game/Misc/DamageReport.hpp>
#include <game/Entities/Bullets/BulletEntityBase.hpp>

////////////////////////////////////////////////////
DamageReport::DamageReport() :
	m_killer	(NULL),
	m_bullet	(NULL),
	m_angle		(0.f),
	m_damage	(0.f)
{
	
}
	
////////////////////////////////////////////////////
DamageReport::DamageReport(sz::Entity* killer, float damage, float angle) :
	m_killer	(killer),
	m_bullet	(NULL),
	m_angle		(angle),
	m_damage	(damage)
{
	
}
	
////////////////////////////////////////////////////
DamageReport::DamageReport(sz::Entity* killer, BulletEntityBase* bullet) :
	m_killer	(killer),
	m_bullet	(NULL),
	m_angle		(0.f),
	m_damage	(0.f)
{
	setBullet(bullet);
}

////////////////////////////////////////////////////
DamageReport::~DamageReport()
{
	
}

////////////////////////////////////////////////////
void DamageReport::setKiller(sz::Entity* entity)
{
	assert(entity);
	m_killer = entity;
}

////////////////////////////////////////////////////
sz::Entity* DamageReport::getKiller()
{
	//assert(m_killer);
	return m_killer;
}

////////////////////////////////////////////////////
void DamageReport::setBullet(BulletEntityBase* bullet)
{
	assert(bullet);
	m_bullet = bullet;

	// Also set angle
	m_angle = sz::getAngle(m_bullet->getComponent<sz::Physics>()->getLinearVelocity());

	m_damage = m_bullet->getDamage();
}

////////////////////////////////////////////////////
BulletEntityBase* DamageReport::getBullet()
{
	//assert(m_bullet);
	return m_bullet;
}

////////////////////////////////////////////////////
void DamageReport::setKillAngle(float angle)
{
	m_angle = angle;
}

////////////////////////////////////////////////////
float DamageReport::getKillAngle()
{
	return m_angle;
}

////////////////////////////////////////////////////
void DamageReport::setDamage(float amount)
{
	m_damage = amount;
}

////////////////////////////////////////////////////
float DamageReport::getDamage()
{
	return m_damage;
}