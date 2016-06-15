#ifndef GAME_BASIC_ENEMY_BULLET_HPP
#define GAME_BASIC_ENEMY_BULLET_HPP

#include <szen/szen.hpp>

#include <game/Entities/Bullets/BulletEntityBase.hpp>

class EnemyBasicBullet : public BulletEntityBase
{
public:
	EnemyBasicBullet();
	~EnemyBasicBullet();

	void onLoad();

	void derivedUpdate();

	/*void begincontact(sz::PhysicsContact&);
	void endcontact(sz::PhysicsContact&);

	bool collisionCallback(sz::PhysicsContact&);*/

	bool onImpact(Entity* hit);
	
private:

	sz::PausableClock lifetimer;

};

#endif // GAME_BASIC_ENEMY_BULLET_HPP
