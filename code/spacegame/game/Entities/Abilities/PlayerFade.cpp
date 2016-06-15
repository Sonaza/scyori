#include <game/Entities/Abilities/PlayerFade.hpp>
#include <game/Entities/Actors/PlayerEntity.hpp>

#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

////////////////////////////////////////////////////
PlayerFade::PlayerFade(PlayerEntity* player) :
	p_player	(player),
	m_active	(false),
	m_fadePhase	(0.f)
{
	loadFromPrefab("data/effects/player_fade.szt");

	m_cooldown		= sf::seconds(0.5f);
	m_zapInterval	= sf::seconds(0.5f);
}

////////////////////////////////////////////////////
PlayerFade::~PlayerFade()
{
	
}

////////////////////////////////////////////////////
float PlayerFade::getPhaseValue()
{
	return m_fadePhase;
}

////////////////////////////////////////////////////
void PlayerFade::update()
{
	if(!p_player->isSpawned()) return;

	//if(m_active && p_player->getStance() != PlayerEntity::Stance::Defensive) deactivate();

	if(p_player->getFlux() <= 0.f) deactivate();

	bool setThings = false;

	if(m_active && m_fadePhase < 1.f)
	{
		m_fadePhase += (1.01f - m_fadePhase) / 12.f;
		setThings = true;

		p_player->removeFlux(p_player->getFluxPercentage(20.f) * Time.delta);

		if(m_zapTimer.getElapsedTime() >= m_zapInterval)
		{
			sf::Vector2f playerPos = p_player->getComponent<sz::Transform>()->getPosition();

			auto enemies = p_player->getComponent<sz::Physics>()->queryRadius(250.f);
			for(auto it = enemies.begin(); it != enemies.end(); ++it)
			{
				EnemyEntityBase* enemy = dynamic_cast<EnemyEntityBase*>(*it);
				if(!enemy) continue;

				float angle = sz::getAngle(playerPos, enemy->getComponent<sz::Transform>()->getPosition());
				enemy->applyDamage(DamageReport(p_player, 60.f, angle));
			}

			m_zapTimer.restart();

			m_zapValue = 1.f;
		}
	}
	else if(m_fadePhase > 0.f)
	{
		m_fadePhase += (-0.01f - m_fadePhase) / 12.f;
		setThings = true;
	}

	m_zapValue += (0.f - m_zapValue) / 9.f;

	getComponent<sz::Renderer>()->setOpacity(m_fadePhase * (0.4f + m_zapValue * 0.3f));

	if(setThings)
	{
		auto transform = p_player->getTransform;
		auto renderer = p_player->getRenderer;

		/*float currentOpacity = renderer->getColor().a / 255.f;
		float currentScale = transform->getScale().x;*/

		float scale = 1.f - m_fadePhase / 4.f;
		transform->setScale(scale, scale);
		renderer->setOpacity(1.f - m_fadePhase * 0.65f);
	}

	auto transform = getComponent<sz::Transform>();
	transform->setScale(1.f + m_zapValue / 1.5f, 1.f + m_zapValue / 1.5f);
}

////////////////////////////////////////////////////
bool PlayerFade::isActive()
{
	return m_active;
}

////////////////////////////////////////////////////
void PlayerFade::activate()
{
	if(!m_active && cooldowned())
	{
		m_active = true;
	}
}

////////////////////////////////////////////////////
void PlayerFade::deactivate()
{
	if(m_active)
	{
		m_active = false;
		m_cooldownTimer.restart();
	}
}

////////////////////////////////////////////////////
bool PlayerFade::cooldowned()
{
	return m_cooldownTimer.getElapsedTime() >= m_cooldown;
}