#include <game/Entities/Actors/Enemies/EnemyBasic.hpp>

#include <game/Entities/Bullets/EnemyBasicBullet.hpp>

////////////////////////////////////////////////////
EnemyBasic::EnemyBasic() :
	m_isShooting	(false),
	m_shootAmount	(0)
{
	m_health.set(100.f);

	loadFromPrefab("data/actors/enemy_basic.szt");

	//getComponent<sz::Physics>()->getBody()->SetLinearDamping(-1.f);
}

////////////////////////////////////////////////////
EnemyBasic::~EnemyBasic()
{
	
}

////////////////////////////////////////////////////
void EnemyBasic::onLoad()
{
	m_shootDelay	= sf::milliseconds(0);//thor::random(800, 1400));
}

////////////////////////////////////////////////////
void EnemyBasic::onDeath(DamageReport& deathdata)
{
	if(deathdata.getKiller() == p_player) spawnFlux(3);

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
void EnemyBasic::update()
{
	updateAI();
}

////////////////////////////////////////////////////
void EnemyBasic::updateAI()
{
	if(p_player->isDead()) return;

	updateMovement(250.f, 600.f, 3.f);

	sf::Vector2f playerPosition = p_player->call(&sz::Transform::getPosition);

	sz::Transform* transform = getComponent<sz::Transform>();
	sf::Vector2f myPosition = transform->getPosition();
	float myangle = sz::toRadians(transform->getRotation());

	float shootdistance = 850.f;
	shootdistance *= shootdistance;

	float distance = sz::distance(myPosition, playerPosition);

	if(distance <= shootdistance)
	{
		if(!m_isShooting && m_shootTimer.getElapsedTime() >= m_shootDelay)
		{
			m_isShooting = true;
			m_shootDelay = sf::milliseconds(thor::random(60, 500));

			m_shootAmount = thor::random(1, 6);
			m_shotCounter = 0;

			m_shootTimer.restart();

			resetBrainfart();
		}
	}

	if(m_isShooting)
	{
		if(m_shootTimer.getElapsedTime() >= m_shootDelay)
		{
			shoot<EnemyBasicBullet>(myangle, 10.f, 70.f);
			m_shootTimer.restart();

			if(++m_shotCounter >= m_shootAmount)
			{
				m_isShooting = false;
				m_shootDelay = sf::milliseconds(thor::random(2000, 3000));
			}
		}
	}
}
