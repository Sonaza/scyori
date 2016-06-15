#include <game/Entities/Actors/Enemies/EnemyRotary.hpp>
#include <game/Entities/Bullets/EnemyRotaryBullet.hpp>

////////////////////////////////////////////////////
EnemyRotary::EnemyRotary() :
	m_isShooting	(false),
	m_shootAmount	(0)
{
	m_health.set(400.f);

	loadFromPrefab("data/actors/enemy_rotary.szt");
}

////////////////////////////////////////////////////
EnemyRotary::~EnemyRotary()
{

}

////////////////////////////////////////////////////
void EnemyRotary::onLoad()
{
	m_shootEffect	= NULL;

	m_shootDelay	= sf::milliseconds(0);
}

////////////////////////////////////////////////////
void EnemyRotary::onDeath(DamageReport& deathdata)
{
	if(deathdata.getKiller() == p_player) spawnFlux(thor::random(8, 10));
	 
	if(m_shootEffect) m_shootEffect->destroyEntity();

	sf::Vector2f position = getComponent<sz::Transform>()->getPosition();

	p_gameGlobals->impact->setColor(sf::Color(153, 230, 255));
	p_gameGlobals->impact->setScale(2.4f, 2.6f);
	p_gameGlobals->impact->setPosition(position);
	p_gameGlobals->impact->emit(1);

	/*p_gameGlobals->explosion->setScale(0.75f, 1.2f);
	p_gameGlobals->explosion->setPosition(position);
	p_gameGlobals->explosion->emit(1);*/

	float angle = deathdata.getKillAngle();

	sz::World::spawn<ExplosionEffect>(this)->setScale(1.4f, 1.9f);
	/*int r = thor::random(1, 2);
	for(int i=0; i < r; ++i)
		sz::World::spawn<SecondaryExplosion>(this, angle)->setScale(0.45f, 0.7f);

	if(thor::random(0, 9) >= 7)
	{
		sz::World::spawn<SecondaryExplosion>(this, angle, 1.5f)->setScale(0.55f, 0.8f);
	}

	p_gameGlobals->impactbits->setColor(sf::Color(153, 230, 255));
	p_gameGlobals->impactbits->setPosition(position);*/

	for(int i=0; i < 40; ++i)
	{
		float v = thor::random(150.f, 750.f);

		p_gameGlobals->impactbits->setVelocityCone(v, angle, thor::random(15.f, 90.f));
		p_gameGlobals->impactbits->emit(1);
	}

	sz::Camera::shake(2.5f);
}
////////////////////////////////////////////////////
void EnemyRotary::update()
{
	/*if(m_shootEffect && !sz::World::exists(m_shootEffect))
	{
		m_shootEffect = NULL;
	}*/

	updateAI();
}

////////////////////////////////////////////////////
void EnemyRotary::updateAI()
{
	if(p_player->isDead()) return;

	updateMovement(600.f, 1200.f, 1.f, 200.f);

	auto anim = getAnimation;

	if(m_isShooting && anim->getCurrentFrame() == 123)
	{
		float baseangle = getTransform->getRotation() + 45.f;

		p_gameGlobals->impactbits->setColor(sf::Color(120, 180, 255));
		p_gameGlobals->impactbits->setScale(0.04f, 0.09f);

		auto pos = getTransform->getPosition();

		for(int i=0; i < 4; ++i)
		{
			float angle = sz::toRadians(baseangle + i * 90.f);
			
			shoot<EnemyRotaryBullet>(angle, 40.f, 50.f);

			auto position = pos + sf::Vector2f(cos(angle + 0.5f), sin(angle + 0.5f)) * 50.f;

			p_gameGlobals->impactbits->setPosition(position);

			for(int i=0; i < 14; ++i)
			{
				float v = thor::random(40.f, 160.f);

				p_gameGlobals->impactbits->setVelocityCone(v, angle, thor::random(0.f, 259.f));
				p_gameGlobals->impactbits->emit(1);
			}

			p_gameGlobals->impact->setColor(sf::Color(120, 180, 255));
			p_gameGlobals->impact->setScale(0.35f, 0.55f);
			p_gameGlobals->impact->setPosition(position);
			p_gameGlobals->impact->emit(1);
		}

		m_shootDelay = sf::milliseconds(thor::random(1500, 4000));

		m_shootTimer.restart();
		m_isShooting = false;
	}

	float shootdistance = 1500.f;
	shootdistance *= shootdistance;

	if(anim->isStopped())
	{
		float distance = sz::distance(getTransform->getPosition(), p_player->getTransform->getPosition());

		if(!m_isShooting && distance <= shootdistance && m_shootTimer.getElapsedTime() >= m_shootDelay)
		{
			anim->play("shoot");

			m_isShooting = true;
		}
		else
		{
			anim->play("idle");
		}
	}
}
