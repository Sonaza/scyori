#include <game/Entities/Actors/Enemies/EnemyBossRed.hpp>

#include <game/Entities/Bullets/EnemyBossRedBullet.hpp>
#include <game/Entities/Actors/Enemies/EnemyLeech.hpp>

#include <game/Entities/Effects/BossBar.hpp>

////////////////////////////////////////////////////
EnemyBossRed::EnemyBossRed() :
	m_isShooting	(false),
	m_shootAmount	(0)
{
	m_health.set(48000.f);

	loadFromPrefab("data/actors/enemy_boss_red.szt");
}

////////////////////////////////////////////////////
EnemyBossRed::~EnemyBossRed()
{
	m_bossBar->destroyEntity();
}

////////////////////////////////////////////////////
void EnemyBossRed::onLoad()
{
	setAsBoss(true);
	p_gameGlobals->setBossSpawned(true);

	m_bossPhase = 0;

	m_damageModifier = 1.f;
	m_scale = 1.f;

	m_bossShield = sz::World::spawn<sz::Entity>();
	m_bossShield->loadFromPrefab("data/effects/boss_red_shield.szt");

	m_bossShield->getRenderer->setOpacity(0.f);
	m_bossShield->getTransform->setInheritRotation(false);

	//////////////////

	m_bossBar = sz::World::spawn<BossBar>(this);

	//////////////////

	p_gameGlobals->impact->setColor(sf::Color(255, 100, 100));
	p_gameGlobals->impact->setScale(5.f, 5.1f);

	p_gameGlobals->impact->setPosition(getTransform->getPosition());
	p_gameGlobals->impact->emit(1);

	sz::Camera::shake(5.f);

	sz::Asset::getSound("big_boom")->play(90.f);

	GameGlobals::get()->addMessage("Zeruldar appears!");
}

////////////////////////////////////////////////////
void EnemyBossRed::onDeath(DamageReport& deathdata)
{
	p_gameGlobals->setBossSpawned(false);

	//spawnFlux(160);

	sf::Vector2f position = getComponent<sz::Transform>()->getPosition();

	p_gameGlobals->impact->setColor(sf::Color(255, 100, 100));
	p_gameGlobals->impact->setScale(3.f, 5.1f);

	for(int i=0; i < 3; ++i)
	{
		sf::Vector2f offset(
			thor::randomDev(0.f, 250.f), thor::randomDev(0.f, 250.f)
		);
		p_gameGlobals->impact->setPosition(position + offset);
		p_gameGlobals->impact->emit(1);
	}

	sz::Asset::getSound("big_boom")->play(90.f);

	sz::Camera::shake(5.f);

	p_gameGlobals->player->applyHeal(p_gameGlobals->player->getEnergyPercentage(100.f));

	/*p_gameGlobals->explosion->setScale(0.5f, 1.f);
	p_gameGlobals->explosion->setPosition(position);
	p_gameGlobals->explosion->emit(1);*/

	sz::World::spawn<ExplosionEffect>(this)->setScale(5.1f, 6.6f);

	float angle = deathdata.getKillAngle();

	p_gameGlobals->impactbits->setColor(sf::Color(255, 100, 40));
	//p_gameGlobals->impactbits->setColor(sf::Color(255, 60, 255));
	p_gameGlobals->impactbits->setPosition(position);

	for(int i=0; i < 500; ++i)
	{
		float v = thor::random(350.f, 750.f);

		p_gameGlobals->impactbits->setVelocityCone(v, angle, 360.f);
		p_gameGlobals->impactbits->emit(1);
	}

	GameGlobals::get()->addMessage("Zeruldar is defeated!");
	GameGlobals::get()->addMessage("You have now beaten");
	GameGlobals::get()->addMessage("everything there is");
	GameGlobals::get()->addMessage("to this game yet.");
	GameGlobals::get()->addMessage("You can keep fighting");
	GameGlobals::get()->addMessage("if you want to.");
	GameGlobals::get()->addMessage("Stay tuned for updates.");
}

////////////////////////////////////////////////////
void EnemyBossRed::update()
{
	updateAI();
}

////////////////////////////////////////////////////
void EnemyBossRed::changePhase(int32 phase)
{
	m_bossPhase = phase;
	m_phaseTimer.restart();
}

////////////////////////////////////////////////////
void EnemyBossRed::basicShootingUpdate()
{
	sf::Time shootCharge = sf::seconds(1.1f - std::min(1.f, (m_damageModifier-1.f) * 0.35f));
	sf::Time shootTime = m_shootTimer.getElapsedTime();

	float progress = std::min(1.f, shootTime.asSeconds() / shootCharge.asSeconds());

	if(m_shootFlashTimer.getElapsedTime() >= sf::milliseconds(50))
	{
		for(int i=-1; i < 3; ++i)
		{
			float angle = m_angle + sz::toRadians(-11.f + i * 22.f);
			sf::Vector2f dir(cos(angle), sin(angle));

			p_gameGlobals->impact->setColor(sf::Color(255, 120, 120));
			p_gameGlobals->impact->setScale(0.3f * progress, 0.5f * progress);
			p_gameGlobals->impact->setPosition(m_position + dir * 280.f * m_scale); 
			p_gameGlobals->impact->emit(1);
		}

		m_shootFlashTimer.restart();
	}

	if(shootTime >= shootCharge)
	{
		for(int i=-1; i < 3; ++i)
		{
			float angle = m_angle + sz::toRadians(-11.f + i * 22.f);

			shoot<EnemyBossRedBullet>(angle, 280.f * m_scale, 70.f);

			sf::Vector2f dir(cos(angle), sin(angle));

			p_gameGlobals->impact->setColor(sf::Color(255, 120, 120));
			p_gameGlobals->impact->setScale(0.7f, 0.8f);
			p_gameGlobals->impact->setPosition(m_position + dir * 280.f * m_scale); 
			p_gameGlobals->impact->emit(1);

			p_gameGlobals->impactbits->setScale(0.02f, 0.1f);
			p_gameGlobals->impactbits->setColor(sf::Color(255, 70, 70));
			p_gameGlobals->impactbits->setPosition(m_position + dir * 280.f * m_scale);

			for(int i=0; i < 30; ++i)
			{
				float v = thor::random(10.f, 50.f);

				p_gameGlobals->impactbits->setVelocityCone(v, angle, 10.f + thor::random(0.f, 20.f));
				p_gameGlobals->impactbits->emit(1);
			}

		}

		getPhysics->accelerate(m_angle-PI, 5.f);

		sz::Asset::getSound("boss_cannon")->play(50.f);

		m_shootTimer.restart();
	}
}

////////////////////////////////////////////////////
void EnemyBossRed::updateAI()
{
	if(p_player->isDead()) return;

	//if(sz::Input::isKeyHit(sf::Keyboard::N)) applyDamage(DamageReport(this, 500000.f, 0.f));
	
	//updateMovement(900.f, 1200.f, 0.5f);

	sf::Vector2f playerPosition = p_player->getTransform->getPosition();

	auto transform = getTransform;
	m_position = transform->getPosition();
	m_angle = sz::toRadians(transform->getRotation());

	float dist = sz::distancesqrt(m_position, playerPosition);

	float angle = sz::getAngle(m_position, playerPosition);

	sf::Vector2f dir(cos(angle), sin(angle));

	auto physics = getPhysics;

	switch(m_bossPhase)
	{
		////////////////////////////////////////
		case 0:
		{
			physics->setRotation(sz::toDegrees(angle));

			if(dist >= 3200.f)
			{
				physics->accelerate(dir * 2.5f);
			}
			else if(dist >= 2200.f)
			{
				physics->accelerate(dir * 1.1f);
			}
			else if(dist <= 800.f)
			{
				physics->accelerate(dir * -0.5f);
			}
			else if(dist <= 1300.f)
			{
				physics->brake(0.05f);
			}

			physics->accelerate(angle + PI * 0.5f, sin(Time.total) * cos(Time.total * 0.25f) * 2.f);

			if(m_phaseTimer.getElapsedTime() >= sf::seconds(10.f))
			{
				changePhase(1);

				m_leechCounter = 0;
			}

			if(Time.total > 1.5f)
				basicShootingUpdate();
		}
		break;

		////////////////////////////////////////
		case 1:
		{
			physics->brake(0.05f);
			physics->setAngularVelocity(physics->getAngularVelocity() + 10.f * Time.delta);

			if(m_leechTimer.getElapsedTime() >= sf::milliseconds(80))
			{
				if(m_leechCounter < 50)
				{
					auto pos = getTransform->getPosition();

					auto e = sz::World::spawn<EnemyLeech>();

					float a = sz::toRadians(m_leechCounter * 24);
					auto o = sf::Vector2f(cos(a), sin(a));

					e->getPhysics->setPosition(pos + o * 300.f);
					e->getPhysics->setLinearVelocity(o * 100.f);

					p_gameGlobals->impact->setColor(sf::Color(255, 90, 90));
					p_gameGlobals->impact->setScale(0.7f, 0.8f);
					p_gameGlobals->impact->setPosition(pos + o * 300.f); 
					p_gameGlobals->impact->emit(1);
				}
				else
				{
					changePhase(2);

					physics->setAngularVelocity(0.f);
					m_shootTimer.restart();
				}

				m_leechTimer.restart();
				m_leechCounter++;
			}
		}
		break;

		////////////////////////////////////////
		case 2:
		{
			float centerAngle = sz::getAngle(m_position, sf::Vector2f(0.f, 0.f));

			physics->accelerate(centerAngle, 5.f);

			float cdist = sz::distancesqrt(m_position, sf::Vector2f(0.f, 0.f));
			if(cdist <= 20.f)
			{
				physics->brake(1.f);
				changePhase(3);

				m_broodTimer.restart();

				m_damageModifier = 1.f;
			}

			physics->setRotation(sz::toDegrees(angle));
			//physics->setAngularVelocity(-physics->getAngularVelocity() /  10.f);

			basicShootingUpdate();
		}
		break;

		////////////////////////////////////////
		case 3:
		{
			physics->brake(0.4f);
			physics->setAngularVelocity(physics->getAngularVelocity() + 2.f * Time.delta);

			sf::Time btime = m_broodTimer.getElapsedTime();

			float dmgscale = log(m_damageModifier * 2.5f) / 5.f;
			float targetScale = 0.5f + dmgscale + (btime.asSeconds() / 10.f) * 0.5f;
			m_scale += (targetScale - m_scale) / 12.f;

			getTransform->setScale(sf::Vector2f(2.5f, 2.5f) * m_scale);

			getRenderer->setOpacity(0.1f + btime.asSeconds() / 10.f * 0.9f);

			if(btime >= sf::seconds(10.f))
			{
				changePhase(0);
				physics->setAngularVelocity(0.f);
			}
		}
		break;
	}

	if(m_bossPhase != 3 && m_damageModifier > 1.f)
	{
		m_damageModifier += (1.f - m_damageModifier) / 10.f * Time.delta;

		float dmgscale = log(m_damageModifier * 2.5f) / 5.f;
		float targetScale = 1.f + dmgscale;
		m_scale += (targetScale - m_scale) / 8.f;

		getTransform->setScale(sf::Vector2f(2.5f, 2.5f) * m_scale);
	}

	{
		float shieldOpacity = m_bossShield->getRenderer->getOpacity();
		if(m_bossPhase == 3)
		{
			m_bossShield->getRenderer->setOpacity(shieldOpacity + (0.8f - shieldOpacity) / 10.f);
			setVulnerability(0.1f);
		}
		else
		{
			m_bossShield->getRenderer->setOpacity(shieldOpacity + (0.f - shieldOpacity) / 10.f);
			setVulnerability(1.f);
		}
	}
}

////////////////////////////////////////////////////
void EnemyBossRed::increaseDmgMod(float v)
{
	if(m_bossPhase == 3)
	{
		m_damageModifier += v;
		m_damageModifier = std::min(5.f, m_damageModifier);
	}
}

////////////////////////////////////////////////////
float EnemyBossRed::getDamageModifier()
{
	return m_damageModifier;
}