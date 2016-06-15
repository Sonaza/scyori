#ifndef GAME_EnemyHeavy_HPP
#define GAME_EnemyHeavy_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

class EnemyHeavy : public EnemyEntityBase
{
public:
	EnemyHeavy();
	~EnemyHeavy();

	void onLoad();

	void update();
	void updateAI();

	void onDeath(DamageReport&);

	sz::PausableClock	m_shootTimer;
	sf::Time			m_shootDelay;

	int32	m_shootAmount;
	int32	m_shotCounter;
	bool	m_isShooting;

};

#endif // GAME_EnemyHeavy_HPP
