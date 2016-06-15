#ifndef GAME_PLAYERBULLET_HPP
#define GAME_PLAYERBULLET_HPP

#include <szen/szen.hpp>

#include <game/Entities/Bullets/BulletEntityBase.hpp>

class PlayerBullet : public BulletEntityBase
{
public:
	PlayerBullet();
	~PlayerBullet();

	void onLoad();

	void derivedUpdate();
	/*
	void begincontact(sz::PhysicsContact&);
	void endcontact(sz::PhysicsContact&);

	bool collisionCallback(sz::PhysicsContact&);*/

	bool onImpact(Entity* hit);
	
private:

	sz::PausableClock lifetimer;

	int32	bulletStance;

};

#endif // GAME_PLAYERBULLET_HPP
