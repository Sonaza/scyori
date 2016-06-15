#include <game/Entities/Actors/PlayerEntity.hpp>

#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

#include <game/Entities/Bullets/PlayerBullet.hpp>

// Player Misc includes
#include <game/Entities/Abilities/PlayerLash.hpp>
#include <game/Entities/Abilities/PlayerCharge.hpp>
#include <game/Entities/Abilities/PlayerFade.hpp>

#include <game/Entities/Effects/PlayerSmoke.hpp>

#include <game/Entities/Effects/EnergyBar.hpp>
#include <game/Entities/Effects/FluxBar.hpp>

#include <game/Entities/Misc/GameMessage.hpp>
#include <game/GameGlobals.hpp>

////////////////////////////////////////////////////
PlayerEntity::PlayerEntity() :
	m_stance		(Stance::Agile)
{
	m_type = Player;

	loadFromPrefab("data/actors/player.szt");

	m_collisionfilter.categoryBits = 0x4;
	//m_collisionfilter.maskBits

	p_gameGlobals->player = this;

	setBeginContactCallback(&PlayerEntity::begincontact);
	setCollisionFilterCallback(&PlayerEntity::collisionCallback);

	m_lastEnergy = m_currentEnergy = 0.f;
	m_lastFlux = m_currentFlux = 0.f;
}

////////////////////////////////////////////////////
PlayerEntity::~PlayerEntity()
{
	
}

////////////////////////////////////////////////////
void PlayerEntity::onLoad()
{
	m_abilityLash	= sz::World::spawn<PlayerLash>(this);
	m_abilityFade	= sz::World::spawn<PlayerFade>(this);

	m_abilityCharge	= sz::World::spawn<PlayerCharge>(this);

	m_smoke = sz::World::spawn<PlayerSmoke>(this);

	{
		m_cannon = sz::World::spawn<PlayerCannon>();
		m_cannon->loadFromPrefab("data/effects/player_cannon.szt");
	}

	{
		m_glow = sz::World::spawn<sz::Entity>();
		m_glow->loadFromPrefab("data/effects/player_glow.szt");
	}

	{
		m_stanceWave = sz::World::spawn<sz::Entity>();
		m_stanceWave->loadFromPrefab("data/effects/player_stance_wave.szt");
	}

	//m_cooldowns.cannon			= sf::milliseconds(125);
	//m_cooldowns.stance_change	= sf::seconds(0.1f);

	/////////////////////////////////
	// Bars

	m_energyBar = sz::World::spawn<EnergyBar>(this);
	m_fluxBar	= sz::World::spawn<FluxBar>(this);

	/////////////////////////////////
	// SOUNDS

	snd_cannon = sz::Asset::getSound("player_cannon");
	//snd_stance_charge = sz::Asset::loadSound("player_stance_charge", "data/snd/player_stance_charge.wav", false);
	//snd_stance_hit = sz::Asset::loadSound("player_stance_hit", "data/snd/player_stance_hit.wav", false);

	snd_stance_charge = sz::Asset::getSound("player_stance_charge");
	snd_stance_hit = sz::Asset::getSound("player_stance_hit");

	/////////////////////////////////

	reset();
}

////////////////////////////////////////////////////
void PlayerEntity::reset()
{
	m_spawnPhase = 0;

	resetEnergy(20000.f);
	resetFlux(100.f);

	switchStance(Stance::Agile);

	m_stanceChargePhase = 0.f;
	m_stanceCharging	= false;

	m_stanceKeyReset	= false;

	getPhysics->brake(1.f);
	getPhysics->setPosition(sf::Vector2f(0.f, 0.f));
	getTransform->setScale(0.f, 0.f);
	getRenderer->setOpacity(0.f);

	m_energyBar->setEnabled(false);
	m_fluxBar->setEnabled(false);

	m_smoke->setIntensity(0.f);
	m_cannon->getTransform->setScale(0.0f, 0.0f);

	m_isDead = false;
}

////////////////////////////////////////////////////
void PlayerEntity::spawn()
{
	m_isDead = false;

	resetEnergy(20000.f);
	resetFlux(100.f);

	m_spawnTimer.restart();
	m_spawnPhase = 1;

	/*getPhysics->setPosition(sf::Vector2f(0.f, 0.f));
	getTransform->setScale(0.f, 0.f);
	getRenderer->setOpacity(0.f);*/

	m_energyBar->setEnabled(true);
	m_fluxBar->setEnabled(true);
}

////////////////////////////////////////////////////
void PlayerEntity::begincontact(sz::PhysicsContact& contact)
{
	auto bullet = dynamic_cast<BulletEntityBase*>(contact.entityB);
	if(bullet)
	{
		applyDamage(bullet->getDamage());
	}
}

////////////////////////////////////////////////////
bool PlayerEntity::collisionCallback(sz::PhysicsContact& contact)
{
	bool isBullet = dynamic_cast<BulletEntityBase*>(contact.entityB) != NULL;
	bool isEnemy = !isBullet && dynamic_cast<EnemyEntityBase*>(contact.entityB) != NULL;

	bool cond = !(m_abilityFade->isActive() && (isBullet || isEnemy)) || !m_abilityFade->isActive();

	return !isDead() && cond;
}

////////////////////////////////////////////////////
void PlayerEntity::update()
{
	if(m_spawnPhase == 2)
	{
		arealimiter();
		handleInput();
		stanceUpdate();
	}

	cameraUpdate();

	miscUpdate();

	if(m_spawnPhase == 1) spawnUpdate();

	m_lastEnergy = m_currentEnergy;
	m_lastFlux = m_currentFlux;

	m_currentEnergy = getEnergyFraction();
	m_currentFlux = getFluxFraction();

	if(m_currentEnergy <= 0.12f && m_lastEnergy > 0.12f)
	{
		GameGlobals::get()->addMessage("Energy nearly depleted!");
	}

	if(m_currentFlux >= 0.75f && m_lastFlux < 0.75f)
	{
		GameGlobals::get()->addMessage("Flux levels rising");
	}
	
	if(m_currentFlux >= 1.f && m_lastFlux < 1.f)
	{
		GameGlobals::get()->addMessage("Flux overload!");
	}

	// DEATH EXPLOSHUN YMS YMS
	if(!m_isDead && m_currentEnergy <= 0.f)
	{
		m_isDead = true;

		auto myPosition = getTransform->getPosition();

		auto query = getPhysics->queryRadius(1300.f);
		for(auto it = query.begin(); it != query.end(); ++it)
		{
			auto enemy = dynamic_cast<EnemyEntityBase*>(*it);
			if(enemy == NULL) continue;

			float angle = sz::getAngle(myPosition, enemy->getTransform->getPosition());

			//enemy->getPhysics->accelerate(angle, (m_stanceFlux * 5.5f + 150.f) * 50.f);
			enemy->applyDamage(DamageReport(NULL, 1000.f, angle));
		}

		p_gameGlobals->impact->setColor(sf::Color(120, 255, 120));
		p_gameGlobals->impact->setScale(4.f, 4.1f);
		
		p_gameGlobals->impact->setPosition(myPosition);
		p_gameGlobals->impact->emit(1);

		m_deathTimer.restart();

		getRenderer->setOpacity(0.f);

		sz::Camera::shake(5.f);

		sz::Asset::getSound("big_boom")->play(90.f);
	}

	if(m_isDead) getPhysics->brake(1.f);

	/*if(sz::Input::isKeyHit(sf::Keyboard::B))
	{
		std::stringstream herp;
		herp << "Kakka message jee " << Time.total;
		GameGlobals::get()->addMessage(herp.str());
	}*/
}

////////////////////////////////////////////////////
bool PlayerEntity::isSpawned()
{
	return m_spawnPhase == 2;
}

////////////////////////////////////////////////////
void PlayerEntity::spawnUpdate()
{
	if(isSpawned() || m_spawnPhase == 0) return;

	float time = m_spawnTimer.getElapsedTime().asSeconds();

	float x = std::max(0.f, time - 0.5f);
	float p = x * x;

	if(time >= 1.5f)
	{
		m_spawnPhase = 2;
		
		getTransform->setScale(1.f, 1.f);
		getRenderer->setOpacity(1.f);
		m_smoke->setIntensity(1.f);

		p_gameGlobals->impact->setColor(sf::Color(120, 255, 120));
		p_gameGlobals->impact->setScale(4.f, 4.1f);

		p_gameGlobals->impact->setPosition(getTransform->getPosition());
		p_gameGlobals->impact->emit(1);

		sz::Camera::shake(5.f);

		sz::Asset::getSound("big_boom")->play(90.f);
	}
	else
	{
		float scale = 0.1f + p * 0.9f;
		float opacity = p;

		getTransform->setScale(scale, scale);
		getRenderer->setOpacity(opacity);

		m_smoke->setIntensity(opacity);
		
		m_glow->getTransform->setScale(scale, scale);
		m_glow->getRenderer->setOpacity(opacity);
	}
}

////////////////////////////////////////////////////
void PlayerEntity::arealimiter()
{
	sf::Vector2f pos = getComponent<sz::Transform>()->getPosition();

	sf::Vector2f zero;
	float dist = sz::distancesqrt(zero, pos);

	if(dist >= 6800.f)
	{
		float angle = sz::getAngle(zero, pos)-PI;

		float x = (dist - 6800.f) / 50.f;

		//getComponent<sz::Physics>()->brake(0.2f);
		getComponent<sz::Physics>()->accelerate(angle, x * x * 2.f + 2.f);
	}
}

////////////////////////////////////////////////////
void PlayerEntity::miscUpdate()
{
	if(isSpawned())
	{
		float cval = cos(Time.total);
		float scale = cval * 2.5f + 3.f;

		m_glow->getTransform->setScale(scale, scale);
		m_glow->getRenderer->setOpacity(cval * -0.4f + 0.5f);
	}

	{
		float scale = 1.f;
		float opacity = 1.f;

		if(m_stanceCharging)
		{
			scale = (0.6f - m_stanceChargePhase * 0.6f) * 1.2f;
			opacity = m_stanceChargePhase * 0.8f;
		}
		else
		{
			scale = (1.4f - m_stanceChargePhase) * 1.2f;
			opacity = m_stanceChargePhase / 1.4f;
		}

		m_stanceWave->getTransform->setScale(scale, scale);
		m_stanceWave->getTransform->setRotation(Time.total * 60.f);
		m_stanceWave->getRenderer->setOpacity(opacity);
	}
}

////////////////////////////////////////////////////
void PlayerEntity::cameraUpdate()
{
	sf::Vector2f target = -call(&sz::Transform::getPosition);
	sf::Vector2f current = sz::Camera::getPosition();

	sf::Vector2f dir = target - current;
	float len = sz::lengthsqrt(dir);
	if(len > 0.f)
	{
		dir = sz::normalize(dir) * len / 10.f;
		sz::Camera::setPosition(current + dir);
	}

	float speed = sz::lengthsqrt(getComponent<sz::Physics>()->getLinearVelocity());

	float targetZoom = 1.8f + std::min(1.f, speed / 130.f) * 1.5f + m_stanceChargePhase * 0.75f;
	if(p_gameGlobals->isBossSpawned()) targetZoom += 1.f;

	float currentZoom = sz::Camera::getZoom();
	
	float zoomDelta = targetZoom - currentZoom;
	sz::Camera::setZoom(currentZoom + zoomDelta / (zoomDelta < 0.f ? 130.f : 70.f));
}

////////////////////////////////////////////////////
void PlayerEntity::handleInput()
{
	handleMovement();
	handleAbilities();
}

////////////////////////////////////////////////////
void PlayerEntity::handleMovement()
{
	if(m_isDead) return;

	// Player movement
	sz::Physics* physics = getComponent<sz::Physics>();

	bool moving = false;

	float modifier = 1.f;

	if(m_abilityCharge->isActive()) return;

	if(m_stance == Stance::Agile)
	{
		physics->setSpeedLimit(160.f);
		modifier = 2.f;
	}
	else
	{
		physics->setSpeedLimit(150.f);
	}

	float speed = 2.f * modifier;

	if(sz::Input::isKeyDown(sf::Keyboard::A))
	{
		physics->accelerate(sf::Vector2f(-speed, 0.f));
		moving = true;
	}

	if(sz::Input::isKeyDown(sf::Keyboard::D))
	{
		physics->accelerate(sf::Vector2f(speed, 0.f));
		moving = true;
	}

	if(sz::Input::isKeyDown(sf::Keyboard::W))
	{
		physics->accelerate(sf::Vector2f(0.f, -speed));
		moving = true;
	}
	if(sz::Input::isKeyDown(sf::Keyboard::S))
	{
		physics->accelerate(sf::Vector2f(0.f, speed));
		moving = true;
	}

	if(moving)
	{
		physics->setLinearDamping(0.f);
	}
	else// if(!m_stanceCharging)
	{
		physics->setLinearDamping(1.f + modifier);
	}
}

////////////////////////////////////////////////////
void PlayerEntity::handleAbilities()
{
	if(m_isDead) return;

	// If player can use cannon
	bool canShoot = true;

	bool stanceKeyDown = sz::Input::isKeyDown(sf::Keyboard::Space);

	if(m_stanceKeyReset && !stanceKeyDown)
	{
		m_stanceKeyReset = false;
	}

	if(!m_stanceCharging && !m_stanceKeyReset && stanceKeyDown)
	{
		if(getFluxFraction() >= 0.05f)
		{
			m_stanceChargePhase = 0.f;
			m_stanceCharging = true;

			m_stanceFlux = getFlux();

			snd_stance_charge->play(60.f);
		}
		else
		{
			GameGlobals::get()->addMessage("Stance change requires flux");
		}
		
		m_stanceKeyReset = true;
	}

	if(m_stanceCharging)
	{
		m_stanceChargePhase += 1.f * Time.delta;

		float energyBonus = 1.f + (1.f - getEnergyFraction()) / 5.f;

		removeFlux(m_stanceFlux * Time.delta);
		applyHeal(getEnergyPercentage(m_stanceFlux * 0.69f * energyBonus) * Time.delta);

		if(!stanceKeyDown ||
			getFlux() <= 0.f ||
			//getEnergyFraction() >= 1.f ||
			m_stanceChargePhase >= 1.f)
		{
			m_stanceCharging = false;
		}
		
		if(!m_stanceCharging)
		{
			if(m_stance == Stance::Agile)	switchStance(Stance::Offensive);
			else							switchStance(Stance::Agile);

			m_stanceChargePhase = 1.4f;
			m_stanceCharging = false;

			//removeFlux(100.f * m_stanceChargePhase);

			auto myPosition = getTransform->getPosition();

			auto query = getPhysics->queryRadius(m_stanceFlux * 5.f + 500.f);
			for(auto it = query.begin(); it != query.end(); ++it)
			{
				auto enemy = dynamic_cast<EnemyEntityBase*>(*it);
				if(enemy == NULL) continue;

				float angle = sz::getAngle(myPosition, enemy->getTransform->getPosition());

				enemy->getPhysics->accelerate(angle, (m_stanceFlux * 5.5f + 150.f) * 50.f);
			}

			if(m_stance == Stance::Agile)
				p_gameGlobals->impact->setColor(sf::Color(120, 255, 120));
			else if(m_stance == Stance::Offensive)
				p_gameGlobals->impact->setColor(sf::Color(255, 120, 120));

			p_gameGlobals->impact->setPosition(myPosition);

			p_gameGlobals->impact->setScale(3.f, 3.f);
			p_gameGlobals->impact->emit(1);
			p_gameGlobals->impact->emit(1);
			p_gameGlobals->impact->emit(1);

			m_stanceKeyReset = true;

			snd_stance_charge->stopAll();
			snd_stance_hit->play(95.f);
		}
	}

	if(!m_stanceCharging)
	{
		m_stanceChargePhase += (0.f - m_stanceChargePhase) / 12.f;
	}

	{
		auto transform = getTransform;

		float scale = 1.f;
		
		if(m_stanceCharging)
		{
			scale = m_stanceChargePhase * -0.25f + 1.f;
		}
		else
		{
			scale = m_stanceChargePhase * 0.65f + 1.f;
		}

		transform->setScale(scale, scale);

		/*auto renderer = getRenderer;

		float opacity = 1.f;
		opacity *= renderer->getColor().a / 255.f;
		//renderer->setOpacity(opacity);*/
	}

	canShoot = canShoot && !m_stanceCharging;

	float lash_cost = getEnergyPercentage(7.f);
	float charge_cost = getEnergyPercentage(7.f);

	float extra_percent = getEnergyPercentage(3.f);
	
	bool pressingFadeButton = sz::Input::isKeyDown(sf::Keyboard::LShift) || sz::Input::isKeyDown(sf::Keyboard::RShift);
	if(!m_stanceCharging && pressingFadeButton)
	{
		m_abilityFade->activate();
	}
	else if(m_abilityFade->isActive())
	{
		m_abilityFade->deactivate();
	}

	canShoot = canShoot && !m_abilityFade->isActive();

	if(canShoot)
	{
		// Abilities per stance
		switch(m_stance)
		{
			////////////////////////////////////////////////////
			case Stance::Agile:
			{
				if(sz::Input::isMouseHit(sf::Mouse::Right) && !m_abilityCharge->isActive() && getEnergy() >= charge_cost + extra_percent)
				{
					m_abilityCharge->activate();
					applyRawDamage(charge_cost);
				}

				break;
			}

			////////////////////////////////////////////////////
			case Stance::Offensive:
			{

				break;
			}
		}

		if(sz::Input::isKeyHit(sf::Keyboard::E) && !m_abilityLash->isLashing() && getEnergy() >= lash_cost + extra_percent)
		{
			m_abilityLash->activate();
			applyRawDamage(lash_cost);
		}
	}
	
	// Stance dance
	/*if(m_stance != Stance::Agile	 &&	sz::Input::isKeyHit(sf::Keyboard::Num1)) switchStance(Stance::Agile);
	if(m_stance != Stance::Defensive && sz::Input::isKeyHit(sf::Keyboard::Num2)) switchStance(Stance::Defensive);
	if(m_stance != Stance::Offensive && sz::Input::isKeyHit(sf::Keyboard::Num3)) switchStance(Stance::Offensive);
	*/

	// Energy cannon ability
	if(canShoot && sz::Input::isMouseDown(sf::Mouse::Left))
	{
		auto cannonTimer = m_cannonTimer.getElapsedTime();
		
		sf::Vector2f pos = call(&sz::Transform::getPosition);
		sf::Vector2f mouse = static_cast<sf::Vector2f>(sz::Input::getMouseWorldPos());

		float angle = sz::getAngle(pos, mouse);

		if(cannonTimer >= m_cooldowns.cannon)
		{
			shoot<PlayerBullet>(angle, 50.f, 110.f);

			m_cannonTimer.restart();

			if(m_stance == Stance::Offensive)
			{
				applyRawDamage(getEnergyPercentage(0.05f));
			}

			auto channel = snd_cannon->play(20.f);
			snd_cannon->setPitch(channel, thor::random(0.8f, 1.2f));
		}

		float progress = std::min(1.f, cannonTimer.asSeconds() / m_cooldowns.cannon.asSeconds()) + 0.1f;

		progress *= progress;

		auto t = m_cannon->getTransform;
		t->setPosition(sf::Vector2f(cos(angle), -sin(angle)) * 65.f);
		t->setRotation(sz::toDegrees(angle));
		t->setScale(0.95f * progress, 0.55f * progress);

		sz::Camera::shake(0.08f);
	}
	else
	{
		m_cannon->getTransform->setScale(0.0f, 0.0f);
	}
}

////////////////////////////////////////////////////
void PlayerEntity::switchStance(const Stance::Type stance)
{
	//if(m_stanceTimer.getElapsedTime() < m_cooldowns.stance_change) return;

	m_stance = stance;
	m_stanceTimer.restart();

	// Restore some energy
	//applyHeal(getEnergyPercentage(35.f));

	switch(m_stance)
	{
		////////////////////////////////////////////////////
		case Stance::Agile:
		{
			m_cooldowns.cannon			= sf::milliseconds(110);
			getComponent<sz::Animation>()->play("idle_green");

			m_glow->getRenderer->setColor(sf::Color(120, 210, 40, 100));
			m_cannon->getRenderer->setColor(sf::Color(160, 255, 100));
		break;
		}

		////////////////////////////////////////////////////
		/*case Stance::Defensive:
		{
				m_cooldowns.cannon			= sf::milliseconds(100);

				getComponent<sz::Animation>()->play("idle_blue");

				m_glow->getComponent<sz::Renderer>()->setColor(sf::Color(40, 120, 210, 100));
			break;
		}*/

		////////////////////////////////////////////////////
		case Stance::Offensive:
		{
			m_cooldowns.cannon			= sf::milliseconds(80);
			getComponent<sz::Animation>()->play("idle_red");
			m_glow->getRenderer->setColor(sf::Color(220, 120, 50, 100));
			m_cannon->getRenderer->setColor(sf::Color(255, 100, 80));
		break;
		}
	}
}

////////////////////////////////////////////////////
void PlayerEntity::stanceUpdate()
{
	if(m_isDead) return;

	float usage_modifier = 1.f;

	// Abilities per stance
	switch(m_stance)
	{
		////////////////////////////////////////////////////
		case Stance::Agile:
		{
			break;
		}

		////////////////////////////////////////////////////
		case Stance::Defensive:
		{
			usage_modifier = 0.4f;
			break;
		}

		////////////////////////////////////////////////////
		case Stance::Offensive:
		{
			usage_modifier = 1.5f;

			break;
		}
	}

	float fluxmod = std::max(0.f, getFluxFraction() - 0.94f);

	usage_modifier *= (fluxmod * fluxmod) * 2470.f + 1.f;

	applyRawDamage(getEnergyPercentage(0.35f) * Time.delta * usage_modifier);

	addFlux(getFluxPercentage(0.65f) * Time.delta);
}

////////////////////////////////////////////////////
void PlayerEntity::applyDamage(float amount)
{
	// Apply stance effects
	if(m_stance == Stance::Defensive) amount *= 0.25f;
	if(m_stance == Stance::Offensive) amount *= 1.5f;
	if(m_abilityFade->isActive()) return;

	float flux = getFluxFraction() * 1.6f;
	amount *= flux * flux + 1.f;

	applyRawDamage(amount);
}

////////////////////////////////////////////////////
void PlayerEntity::applyRawDamage(float amount)
{
	setEnergy(m_energy.current - amount);
}

////////////////////////////////////////////////////
void PlayerEntity::applyHeal(float amount)
{
	//m_energy.current = std::min(m_energy.maximum, m_energy.current + amount);
	setEnergy(m_energy.current + amount);
}

////////////////////////////////////////////////////
void PlayerEntity::setEnergy(float amount)
{
	m_energy.current = std::min(m_energy.maximum, std::max(-1.f, amount));
}

////////////////////////////////////////////////////
void PlayerEntity::resetEnergy()
{
	m_energy.current = m_energy.maximum;
}

////////////////////////////////////////////////////
void PlayerEntity::resetEnergy(float amount)
{
	m_energy.current = m_energy.maximum = amount;
}

////////////////////////////////////////////////////
void PlayerEntity::setFlux(float amount)
{
	m_flux.current = std::min(m_flux.maximum, std::max(0.f, amount));
}

////////////////////////////////////////////////////
void PlayerEntity::resetFlux()
{
	m_flux.current = 0.f;
}

////////////////////////////////////////////////////
void PlayerEntity::resetFlux(float amount)
{
	m_flux.current = 0.f;
	m_flux.maximum = amount;
}

////////////////////////////////////////////////////
void PlayerEntity::addFlux(float amount)
{
	 setFlux(m_flux.current + amount);
}

////////////////////////////////////////////////////
void PlayerEntity::removeFlux(float amount)
{
	setFlux(m_flux.current - amount);
}

////////////////////////////////////////////////////
float PlayerEntity::getDamageModifier()
{
	float modifier = 1.f;

	switch(m_stance)
	{
		case Stance::Agile:		modifier = 1.f;		break;
		case Stance::Defensive: modifier = 0.67f;	break;
		case Stance::Offensive: modifier = 1.25f;	break;
	}

	//modifier *= 2.f - getEnergyFraction();

	float flux = getFluxFraction();
	modifier *= flux * flux + 1.f;

	return 1.f * modifier;
}