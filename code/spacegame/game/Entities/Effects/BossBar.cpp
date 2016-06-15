#include <game/Entities/Effects/BossBar.hpp>

#include <game/GameGlobals.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

////////////////////////////////////////////////////
BossBar::BossBar(EnemyEntityBase* boss) :
	m_boss		(boss),
	m_current	(1.f)
{
	loadFromPrefab("data/boss_bar.szt");

	m_bossText = sz::World::spawn<sz::Entity>();
	m_bossText->loadFromPrefab("data/boss_text.szt");

	m_bossName = sz::World::spawn<sz::Entity>();
	m_bossName->loadFromPrefab("data/boss_name.szt");

	m_bossName->getTextRenderer->setText("Zeruldar");
}

////////////////////////////////////////////////////
BossBar::~BossBar()
{
	m_bossText->destroyEntity();
	m_bossName->destroyEntity();
}

////////////////////////////////////////////////////
void BossBar::update()
{
	if(!GameGlobals::get()->isBossSpawned())
	{
		destroyEntity();
		return;
	}

	m_current += (m_boss->getHealthFraction() - m_current) / 5.f;
	m_current = std::max(0.f, m_current);

	char s[100];
	sprintf(s, "Boss %.1f%%", m_current * 100.f);

	m_bossText->getTextRenderer->setText(s);

	auto shader = getRenderer->getShader();
	if(shader)
	{
		shader->setParameter("u_fluxbar", -100);
		shader->setParameter("u_progress", m_current * 0.45f);
	}
}