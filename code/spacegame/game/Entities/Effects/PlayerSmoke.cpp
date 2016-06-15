#include <game/Entities/Effects/PlayerSmoke.hpp>
#include <game/Entities/Abilities/PlayerFade.hpp>
#include <game/GameGlobals.hpp>

////////////////////////////////////////////////////
PlayerSmoke::PlayerSmoke(PlayerEntity* player) :
	m_player		(player),
	m_intensity		(0.f)
{
	loadFromPrefab("data/effects/player_smoke.szt");
}

////////////////////////////////////////////////////
PlayerSmoke::~PlayerSmoke()
{
	
}

////////////////////////////////////////////////////
void PlayerSmoke::setIntensity(float intensity)
{
	m_intensity = intensity;
}

////////////////////////////////////////////////////
void PlayerSmoke::update()
{
	float speed = sz::lengthsqrt(m_player->getComponent<sz::Physics>()->getLinearVelocity());

	sz::ParticleComponent* particle = getComponent<sz::ParticleComponent>();
	particle->setEmissionRate(speed / 2.5f);

	PlayerEntity::Stance::Type stance = m_player->getStance();

	sz::Renderer* renderer = getComponent<sz::Renderer>();

	if(stance != m_stance)
	{
		m_stance = stance;

		// Change colors when stance is changed
		switch(m_stance)
		{
			case PlayerEntity::Stance::Agile:
				renderer->setColor(sf::Color(120, 210, 40));
				particle->setColor(sf::Color(120, 220, 40));
			break;

			case PlayerEntity::Stance::Defensive:
				renderer->setColor(sf::Color(30, 100, 240));
				particle->setColor(sf::Color(30, 100, 240));
			break;

			case PlayerEntity::Stance::Offensive:
				renderer->setColor(sf::Color(255, 120, 120));
				particle->setColor(sf::Color(255, 90, 90));
			break;
		}
	}

	float phase = m_player->m_abilityFade->getPhaseValue();

	float scale = 1.7f + phase * 0.5f;
	scale *= m_intensity;

	float opacity = 1.f - phase / 1.4f;
	opacity *= m_intensity;

	getComponent<sz::Transform>()->setScale(scale, scale);
	renderer->setOpacity(opacity);
}
