#ifndef GAME_DEATHDATA_HPP
#define GAME_DEATHDATA_HPP

#include <szen/szen.hpp>

class BulletEntityBase;

class DamageReport
{
public:
	DamageReport();
	DamageReport(sz::Entity* killer, BulletEntityBase* bullet);
	DamageReport(sz::Entity* killer, float damage, float angle_radians);

	~DamageReport();

	void setKiller(sz::Entity* killer);
	sz::Entity* getKiller();

	void setBullet(BulletEntityBase* bullet);
	BulletEntityBase* getBullet();

	void setKillAngle(float radians);
	float getKillAngle();

	void setDamage(float amount);
	float getDamage();

private:

	sz::Entity*			m_killer;

	BulletEntityBase*	m_bullet;

	float	m_damage;

	bool	m_hasAngle;
	float	m_angle; // RADIAANEJA

};

#endif // GAME_DEATHDATA_HPP
