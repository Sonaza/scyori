#include <game/Entities/Abilities/PlayerCharge.hpp>
#include <game/GameGlobals.hpp>

#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

////////////////////////////////////////////////////
PlayerCharge::PlayerCharge(PlayerEntity* player) :
	m_player		(player),
	m_active		(false),
	m_opacity		(0.f)
{
	loadFromPrefab("data/effects/player_charge.szt");
}

////////////////////////////////////////////////////
PlayerCharge::~PlayerCharge()
{
	
}

////////////////////////////////////////////////////
void PlayerCharge::onLoad()
{
	m_sndCharge = sz::Asset::getSound("charge");
}

////////////////////////////////////////////////////
void PlayerCharge::update()
{
	if(m_active)
	{
		auto physics = m_player->getPhysics;

		physics->setLinearDamping(0.f);
		physics->setSpeedLimit(0.f);
		//physics->accelerate(m_direction * 15.f);

		std::vector<Entity*> enemies = m_player->getPhysics->queryRadius(190.f);

		auto position = getTransform->getPosition();

		for(auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			auto enemy = dynamic_cast<EnemyEntityBase*>(*it);
			if(!enemy) continue;

			auto enemyPos = enemy->call(&sz::Transform::getPosition);
			float angle = sz::getAngle(position, enemyPos);

			if(enemy->applyDamage(DamageReport(m_player, 300.f, angle)))
			{
				sz::Camera::shake(1.f);
			}
		}

		if(m_timer.getElapsedTime() >= sf::seconds(0.35f))
		{
			physics->brake(0.95f);

			auto impact = GameGlobals::get()->impact;

			impact->setPosition(position);

			impact->setColor(sf::Color(120, 195, 40));
			impact->setScale(1.5f, 2.f);
			impact->emit(1);
			/*
			impact->setColor(sf::Color(250, 210, 40));
			impact->setScale(3.5f, 4.f);
			impact->emit(1);*/

			sz::Camera::shake(3.f);

			m_active = false;

			getParticle->setEmissionRate(0.f);
		}
	}

	m_opacity += ((m_active ? 1.f : 0.f) - m_opacity) / (m_active ? 4.f : 8.f);
	getRenderer->setOpacity(m_opacity * 0.15f);

	auto scale = m_player->getTransform->getScale().x;
	float s = scale * (1.f - m_opacity * 0.3f);
	m_player->getTransform->setScale(s, s);
}

////////////////////////////////////////////////////
void PlayerCharge::activate()
{
	if(m_active) return;

	m_sndCharge->play();

	m_active = true;

	m_timer.restart();

	sf::Vector2f mouse = static_cast<sf::Vector2f>(sz::Input::getMouseWorldPos());
	sf::Vector2f player = m_player->getTransform->getPosition();

	float angle = sz::getAngle(player, mouse);

	m_direction = sf::Vector2f(
		cos(angle), sin(angle)
	);

	getTransform->setRotation(sz::toDegrees(angle));

	auto physics = m_player->getPhysics;

	physics->setLinearDamping(0.f);
	physics->brake(0.99f);
	physics->setSpeedLimit(0.f);
	physics->accelerate(m_direction * 120.f);

	m_opacity = 0.f;

	//getRenderer->setOpacity(0.7f);
	getParticle->setEmissionRate(120.f);
	getParticle->setRotation(sz::toDegrees(angle));
}

////////////////////////////////////////////////////
bool PlayerCharge::isActive()
{
	return m_active;
}