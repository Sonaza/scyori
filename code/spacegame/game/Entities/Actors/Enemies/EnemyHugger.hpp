#ifndef GAME_EnemyHugger_HPP
#define GAME_EnemyHugger_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

class EnemyHugger : public EnemyEntityBase
{
public:
	EnemyHugger();
	~EnemyHugger();

	void update();
	void updateAI();

	void onDeath(DamageReport&);

	//sz::PausableClock	m_shootTimer;

	bool				m_isArmed;
	sf::Time			m_popTime;
	sz::PausableClock	m_popTimer;

	sz::PausableClock	m_trailTimer;

};

#endif // GAME_EnemyHugger_HPP
