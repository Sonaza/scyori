#ifndef GAME_ENEMYROTARY_HPP
#define GAME_ENEMYROTARY_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

class EnemyRotary : public EnemyEntityBase
{
public:
	EnemyRotary();
	~EnemyRotary();

	void onLoad();

	void update();
	void updateAI();

	void onDeath(DamageReport&);

	sz::PausableClock	m_shootTimer;
	sf::Time			m_shootDelay;

	int32	m_shootAmount;
	int32	m_shotCounter;
	bool	m_isShooting;

	sz::Entity*			m_shootEffect;

};

#endif // GAME_ENEMYROTARY_HPP
