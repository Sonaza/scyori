#ifndef GAME_SecondaryExplosion_HPP
#define GAME_SecondaryExplosion_HPP

#include <szen/szen.hpp>

class SecondaryExplosion : public sz::Entity
{
public:
	SecondaryExplosion(sz::Entity* parent, float angle, float speed = 1.f);
	~SecondaryExplosion();

	void update();

	void setScale(float min, float max);

private:

	sz::Entity*	m_parent;

	sz::PausableClock	m_trailTimer;

	sf::Time			m_time;
	sz::PausableClock	m_timer;

	sf::Vector2f		m_scale;

	sf::Vector2f		m_direction;

	bool	m_exploded;

};

#endif // GAME_SecondaryExplosion_HPP
