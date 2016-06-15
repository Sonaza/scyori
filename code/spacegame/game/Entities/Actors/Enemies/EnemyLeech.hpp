#ifndef GAME_EnemyLeech_HPP
#define GAME_EnemyLeech_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

class EnemyLeech : public EnemyEntityBase
{
public:
	EnemyLeech();
	~EnemyLeech();

	void update();
	void updateAI();

	void onDeath(DamageReport&);

	sz::PausableClock	m_leechTimer;

	bool				m_isArmed;
	sf::Time			m_popTime;
	sz::PausableClock	m_popTimer;

	sz::PausableClock	m_trailTimer;

};

#endif // GAME_EnemyLeech_HPP
