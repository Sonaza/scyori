#ifndef GAME_EnemyBasic_HPP
#define GAME_EnemyBasic_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

class EnemyBasic : public EnemyEntityBase
{
public:
	EnemyBasic();
	~EnemyBasic();

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

#endif // GAME_EnemyBasic_HPP
