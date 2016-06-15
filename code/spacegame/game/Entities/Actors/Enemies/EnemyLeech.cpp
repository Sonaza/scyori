#include <game/Entities/Actors/Enemies/EnemyLeech.hpp>

////////////////////////////////////////////////////
EnemyLeech::EnemyLeech() :
	m_isArmed	(false)
{
	m_health.set(150.f);

	loadFromPrefab("data/actors/enemy_leech.szt");

	m_popTime = sf::seconds(0.5f);
}

////////////////////////////////////////////////////
EnemyLeech::~EnemyLeech()
{
	
}

////////////////////////////////////////////////////
void EnemyLeech::onDeath(DamageReport& deathdata)
{
	if(thor::random(0, 2)) spawnFlux(2);

	sf::Vector2f position = getComponent<sz::Transform>()->getPosition();

	p_gameGlobals->impact->setColor(sf::Color(255, 80, 80));
	p_gameGlobals->impact->setScale(1.2f, 1.8f);

	p_gameGlobals->impact->setPosition(position);
	p_gameGlobals->impact->emit(1);

	sz::World::spawn<ExplosionEffect>(this)->setScale(1.3f, 1.9f);

	float angle = deathdata.getKillAngle();//sz::getAngle(vel);

	p_gameGlobals->impactbits->setColor(sf::Color(255, 90, 90));
	p_gameGlobals->impactbits->setPosition(position);
	p_gameGlobals->impactbits->setScale(0.08f, 0.12f);

	const int nbparticles = thor::random(10, 20);
	for(int i=0; i < nbparticles; ++i)
	{
		float v = thor::random(150.f, 750.f);

		p_gameGlobals->impactbits->setVelocityCone(v, angle, thor::random(15.f, 80.f));
		p_gameGlobals->impactbits->emit(1);
	}

	sz::Camera::shake(1.f);
}

////////////////////////////////////////////////////
void EnemyLeech::update()
{
	updateAI();

	//if(m_trailTimer.getElapsedTime() >= sf::milliseconds(5))
	{
		m_trailTimer.restart();

		p_gameGlobals->impactbits->setColor(sf::Color(255, 30, 30));
		p_gameGlobals->impactbits->setPosition(getTransform->getPosition());
		p_gameGlobals->impactbits->setScale(0.08f, 0.12f);

		float angle = sz::toRadians(getTransform->getRotation()-180.f);

		p_gameGlobals->impactbits->setVelocityCone(30.f, angle, 0.f);
		p_gameGlobals->impactbits->emit(1);
	}
}

////////////////////////////////////////////////////
void EnemyLeech::updateAI()
{
	if(p_player->isDead()) return;

	updateMovement(1.f, 100.f, 30.f, -1.f);

	sf::Vector2f playerPosition = p_player->call(&sz::Transform::getPosition);

	sz::Transform* transform = getComponent<sz::Transform>();
	sf::Vector2f myPosition = transform->getPosition();
	//float myangle = sz::toRadians(transform->getRotation());

	float ddist = 140.f;
	ddist *= ddist;

	float dist = sz::distance(myPosition, playerPosition);

	if(dist <= ddist && m_leechTimer.getElapsedTime() >= sf::milliseconds(330))
	{
		p_player->applyDamage(15.f);
		m_leechTimer.restart();
	}
}
