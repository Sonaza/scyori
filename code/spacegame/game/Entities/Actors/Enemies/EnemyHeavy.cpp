#include <game/Entities/Actors/Enemies/EnemyHeavy.hpp>

#include <game/Entities/Bullets/EnemyHeavyBullet.hpp>

////////////////////////////////////////////////////
EnemyHeavy::EnemyHeavy() :
	m_isShooting	(false),
	m_shootAmount	(0)
{
	m_health.set(200.f);

	loadFromPrefab("data/actors/enemy_heavy.szt");

	//getComponent<sz::Physics>()->getBody()->SetLinearDamping(-1.f);
}

////////////////////////////////////////////////////
EnemyHeavy::~EnemyHeavy()
{
	
}

////////////////////////////////////////////////////
void EnemyHeavy::onLoad()
{
	m_shootDelay	= sf::milliseconds(0);//thor::random(800, 1400));
}

////////////////////////////////////////////////////
void EnemyHeavy::onDeath(DamageReport& deathdata)
{
	if(deathdata.getKiller() == p_player) spawnFlux(thor::random(5, 7));

	sf::Vector2f position = getComponent<sz::Transform>()->getPosition();

	p_gameGlobals->impact->setColor(sf::Color(250, 120, 250));
	p_gameGlobals->impact->setScale(2.2f, 2.6f);
	p_gameGlobals->impact->setPosition(position); 
	p_gameGlobals->impact->emit(1);

	/*p_gameGlobals->explosion->setScale(0.5f, 1.f);
	p_gameGlobals->explosion->setPosition(position);
	p_gameGlobals->explosion->emit(1);*/

	sz::World::spawn<ExplosionEffect>(this)->setScale(1.1f, 1.6f);

	float angle = deathdata.getKillAngle();

	p_gameGlobals->impactbits->setColor(sf::Color(255, 100, 40));
	//p_gameGlobals->impactbits->setColor(sf::Color(255, 60, 255));
	p_gameGlobals->impactbits->setPosition(position);

	const int nbparticles = thor::random(20, 40);
	for(int i=0; i < nbparticles; ++i)
	{
		float v = thor::random(150.f, 750.f);

		p_gameGlobals->impactbits->setVelocityCone(v, angle, thor::random(15.f, 30.f));
		p_gameGlobals->impactbits->emit(1);

	}	

	sz::Camera::shake(1.5f);
}
////////////////////////////////////////////////////
void EnemyHeavy::update()
{
	updateAI();
}

////////////////////////////////////////////////////
void EnemyHeavy::updateAI()
{
	if(p_player->isDead()) return;
	
	updateMovement(250.f, 600.f, 2.f);

	sf::Vector2f playerPosition = p_player->call(&sz::Transform::getPosition);

	sz::Transform* transform = getComponent<sz::Transform>();
	sf::Vector2f myPosition = transform->getPosition();
	float myangle = sz::toRadians(transform->getRotation());

	float shootdistance = 600.f;
	shootdistance *= shootdistance;

	float distance = sz::distance(myPosition, playerPosition);

	if(distance <= shootdistance)
	{
		if(!m_isShooting && m_shootTimer.getElapsedTime() >= m_shootDelay)
		{
			m_isShooting = true;
			m_shootDelay = sf::milliseconds(thor::random(40, 500));

			m_shootAmount = thor::random(2, 3);
			m_shotCounter = 0;

			m_shootTimer.restart();

			resetBrainfart();
		}
	}

	if(m_isShooting)
	{
		if(m_shootTimer.getElapsedTime() >= m_shootDelay)
		{
			shoot<EnemyHeavyBullet>(myangle, 15.f, 40.f);
			m_shootTimer.restart();

			if(++m_shotCounter >= m_shootAmount)
			{
				m_isShooting = false;
				m_shootDelay = sf::milliseconds(thor::random(2000, 3000));
			}
		}
	}
}
