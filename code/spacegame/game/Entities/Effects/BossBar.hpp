#ifndef GAME_BossBar_HPP
#define GAME_BossBar_HPP

#include <szen/szen.hpp>

class EnemyEntityBase;

class BossBar : public sz::Entity
{
public:
	BossBar(EnemyEntityBase* boss);
	~BossBar();

	void update();

private:

	sz::Entity*		m_bossText;
	sz::Entity*		m_bossName;

	EnemyEntityBase*	m_boss;

	float			m_current;

};

#endif // GAME_BossBar_HPP
