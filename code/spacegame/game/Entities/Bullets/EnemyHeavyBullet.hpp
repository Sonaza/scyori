#ifndef GAME_HEAVY_ENEMY_BULLET_HPP
#define GAME_HEAVY_ENEMY_BULLET_HPP

#include <szen/szen.hpp>

#include <game/Entities/Bullets/BulletEntityBase.hpp>

class EnemyHeavyBullet : public BulletEntityBase
{
public:
	EnemyHeavyBullet();
	~EnemyHeavyBullet();

	void onLoad();

	void derivedUpdate();

	/*void begincontact(sz::PhysicsContact&);
	void endcontact(sz::PhysicsContact&);

	bool collisionCallback(sz::PhysicsContact&);*/

	bool onImpact(Entity* hit);
	
private:

	sz::PausableClock lifetimer;

};

#endif // GAME_HEAVY_ENEMY_BULLET_HPP
