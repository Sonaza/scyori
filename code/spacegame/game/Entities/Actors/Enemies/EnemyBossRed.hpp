#ifndef GAME_EnemyBossRed_HPP
#define GAME_EnemyBossRed_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

class BossBar;

class EnemyBossRed : public EnemyEntityBase
{
public:
	EnemyBossRed();
	~EnemyBossRed();

	void onLoad();

	void update();
	void updateAI();

	void onDeath(DamageReport&);

	void changePhase(int32 phase);

	float getDamageModifier();

	void handlePhaseA();

	void basicShootingUpdate();

	void increaseDmgMod(float v);

	BossBar*		m_bossBar;

	sz::Entity*		m_bossShield;

	int32	m_bossPhase;
	sz::PausableClock	m_phaseTimer;

	float			m_angle;
	sf::Vector2f	m_position;

	float			m_scale;
	sz::PausableClock	m_broodTimer;

	//////////////////////////////

	sz::PausableClock	m_shootFlashTimer;

	sz::PausableClock	m_shootTimer;
	sf::Time			m_shootDelay;

	int32	m_shootAmount;
	int32	m_shotCounter;
	bool	m_isShooting;

	sz::PausableClock	m_leechTimer;
	int32				m_leechCounter;

	float m_damageModifier;

};

#endif // GAME_EnemyBossRed_HPP
