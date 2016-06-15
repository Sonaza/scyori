#ifndef GAME_FLUXICLE_HPP
#define GAME_FLUXICLE_HPP

#include <szen/szen.hpp>

class PlayerEntity;

class Fluxicle : public sz::Entity
{
public:
	Fluxicle(sz::Entity* atEntity);
	~Fluxicle();

	void onLoad();
	void update();

	void setSpeed(float val);
	void setAngle(float radians);

private:

	PlayerEntity*	m_player;

	float	m_speed;
	float	m_angle;

	sf::Time		m_pullDelay;

};

#endif // GAME_FLUXICLE_HPP
