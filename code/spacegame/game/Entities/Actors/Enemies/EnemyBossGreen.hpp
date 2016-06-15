#ifndef GAME_EnemyBossGreen_HPP
#define GAME_EnemyBossGreen_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

class EnemyBossGreen : public EnemyEntityBase
{
public:
	EnemyBossGreen();
	~EnemyBossGreen();

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

#endif // GAME_EnemyBossGreen_HPP
