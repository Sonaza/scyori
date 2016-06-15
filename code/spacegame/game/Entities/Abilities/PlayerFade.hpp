#ifndef GAME_PLAYERFADE_HPP
#define GAME_PLAYERFADE_HPP

#include <szen/szen.hpp>

class PlayerEntity;

class PlayerFade : public sz::Entity
{
public:
	PlayerFade(PlayerEntity*);
	~PlayerFade();

	void update();

	bool isActive();
	void activate();
	void deactivate();

	float getPhaseValue();

private:

	bool cooldowned();

	bool	m_active;

	float	m_fadePhase;

	sf::Time			m_cooldown;
	sz::PausableClock	m_cooldownTimer;

	sf::Time			m_zapInterval;
	sz::PausableClock	m_zapTimer;
	float				m_zapValue;

	PlayerEntity*		p_player;

};

#endif // GAME_PLAYERFADE_HPP
