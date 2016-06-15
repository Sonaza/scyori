#ifndef GAME_ENEMY_ROTARY_BULLET_HPP
#define GAME_ENEMY_ROTARY_BULLET_HPP

#include <szen/szen.hpp>

#include <game/Entities/Bullets/BulletEntityBase.hpp>

class EnemyBossRed;

class EnemyBossRedBullet : public BulletEntityBase
{
public:
	EnemyBossRedBullet();
	~EnemyBossRedBullet();

	void onLoad();

	void derivedUpdate();

	/*void begincontact(sz::PhysicsContact&);
	void endcontact(sz::PhysicsContact&);

	bool collisionCallback(sz::PhysicsContact&);*/

	bool onImpact(Entity* hit);
	
private:

	EnemyBossRed* m_bossPointer;

	sz::PausableClock lifetimer;

};

#endif // GAME_ENEMY_ROTARY_BULLET_HPP
