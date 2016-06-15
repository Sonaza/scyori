#include <game/Entities/Actors/Enemies/EnemyHugger.hpp>

////////////////////////////////////////////////////
EnemyHugger::EnemyHugger() :
	m_isArmed	(false)
{
	m_health.set(90.f);

	loadFromPrefab("data/actors/enemy_hugger.szt");

	m_popTime = sf::seconds(0.5f);
}

////////////////////////////////////////////////////
EnemyHugger::~EnemyHugger()
{
	
}

////////////////////////////////////////////////////
void EnemyHugger::onDeath(DamageReport& deathdata)
{
	if(deathdata.getKiller() == p_player) spawnFlux(6);

	sf::Vector2f position = getComponent<sz::Transform>()->getPosition();

	p_gameGlobals->impact->setColor(sf::Color(255, 190, 20));
	if(m_isArmed)
		p_gameGlobals->impact->setScale(3.8f, 4.5f);
	else
		p_gameGlobals->impact->setScale(2.2f, 2.8f);

	p_gameGlobals->impact->setPosition(position);
	p_gameGlobals->impact->emit(1);

	sz::World::spawn<ExplosionEffect>(this)->setScale(1.3f, 1.9f);

	float angle = deathdata.getKillAngle();//sz::getAngle(vel);

	p_gameGlobals->impactbits->setColor(sf::Color(255, 190, 20));
	p_gameGlobals->impactbits->setPosition(position);
	p_gameGlobals->impactbits->setScale(0.08f, 0.12f);

	const int nbparticles = thor::random(30, 40);
	for(int i=0; i < nbparticles; ++i)
	{
		float v = thor::random(150.f, 750.f);

		p_gameGlobals->impactbits->setVelocityCone(v, angle, thor::random(15.f, 80.f));
		p_gameGlobals->impactbits->emit(1);
	}

	sz::Camera::shake(1.f);
}

////////////////////////////////////////////////////
void EnemyHugger::update()
{
	updateAI();

	//if(m_trailTimer.getElapsedTime() >= sf::milliseconds(5))
	{
		m_trailTimer.restart();

		p_gameGlobals->impactbits->setColor(sf::Color(255, 190, 20));
		p_gameGlobals->impactbits->setPosition(getTransform->getPosition());
		p_gameGlobals->impactbits->setScale(0.08f, 0.12f);

		float angle = sz::toRadians(getTransform->getRotation()-180.f);

		for(int i=0; i < 2; ++i)
		{
			float v = thor::random(0.f, 1.f);

			p_gameGlobals->impactbits->setVelocityCone(v, angle, 0.f);
			p_gameGlobals->impactbits->emit(1);
		}
	}

	//PlayerEntity* player = dynamic_cast<PlayerEntity*>(p_gameGlobals->player);

	/*sf::Vector2f playerPosition = p_player->call(&sz::Transform::getPosition);

	sz::Transform* transform = getComponent<sz::Transform>();
	sf::Vector2f myPosition = transform->getPosition();
	float myangle = sz::toRadians(transform->getRotation());

	float shootdistance = 1200.f;
	shootdistance *= shootdistance;

	float distance = sz::distance(myPosition, playerPosition);

	if(distance <= shootdistance && m_shootTimer.getElapsedTime().asMilliseconds() > 1000)
	{
		shoot<EnemyBasicBullet>(myangle, 10.f, 70.f);
		m_shootTimer.restart();

		getComponent<sz::Animation>()->play("shoot");
	}*/
}

////////////////////////////////////////////////////
void EnemyHugger::updateAI()
{
	if(p_player->isDead()) return;
	
	updateMovement(1.f, 100.f, !m_isArmed ? 11.f : 0.75f, 10.f);

	sf::Vector2f playerPosition = p_player->call(&sz::Transform::getPosition);

	sz::Transform* transform = getComponent<sz::Transform>();
	sf::Vector2f myPosition = transform->getPosition();

	float distance = sz::distance(myPosition, playerPosition);

	if(!m_isArmed)
	{
		if(distance <= 65000.f)
		{
			m_isArmed = true;
			m_popTimer.restart();
		}

		if(m_popTimer.getElapsedTime() >= sf::seconds(15.f))
		{
			applyDamage(DamageReport(this, 100.f, sz::toRadians(transform->getRotation()-180.f)));
		}
	}
	else if(m_isArmed)
	{
		auto scale = transform->getScale().x + 0.5f * Time.delta;
		transform->setScale(scale, scale);

		if(m_popTimer.getElapsedTime() >= m_popTime)
		{
			auto nearby = getPhysics->queryRadius(200.f);

			for(auto it = nearby.begin(); it != nearby.end(); ++it)
			{
				auto enemy = dynamic_cast<EnemyEntityBase*>(*it);
				if(!enemy || enemy == this || enemy->isBoss()) continue;

				float angle = sz::getAngle(myPosition, enemy->getTransform->getPosition());
				enemy->applyDamage(DamageReport(this, 150.f, angle));
			}

			if(distance <= 65000.f)
			{
				float angle = sz::getAngle(myPosition, playerPosition);
				p_player->applyDamage(1000.f);
				//p_player->getPhysics->accelerate(angle, 800.f);

				sz::Camera::shake(4.f);
			}
			else
			{
				sz::Camera::shake(1.f);
			}

			applyDamage(DamageReport(this, 100.f, sz::toRadians(transform->getRotation()-180.f)));

		}
	}
}
