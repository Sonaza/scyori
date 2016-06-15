#include <game/Entities/Effects/SecondaryExplosion.hpp>
#include <game/Entities/Effects/ExplosionEffect.hpp>

////////////////////////////////////////////////////
SecondaryExplosion::SecondaryExplosion(sz::Entity* parent, float ba, float spd) :
	m_parent	(parent),
	m_exploded	(false)
{
	loadFromPrefab("data/effects/explosion2nd.szt");
	
	auto pos = m_parent->getTransform->getPosition();
	pos.y *= -1.f;
	getTransform->setPosition(pos);

	float angle = thor::randomDev(ba+0.05f, 0.4f);
	float speed = thor::random(580.f, 1250.f) * spd;

	m_direction.x = cos(angle) * speed;
	m_direction.y = sin(angle) * speed;

	m_time = sf::milliseconds(thor::random(100, 350));

	getParticle->setEmissionRate(0);
}

////////////////////////////////////////////////////
SecondaryExplosion::~SecondaryExplosion()
{
	
}

////////////////////////////////////////////////////
void SecondaryExplosion::update()
{
	getTransform->move(m_direction * Time.delta);

	if(m_trailTimer.getElapsedTime() >= sf::seconds(m_time.asSeconds() / 15.f))
	{
		float t = (m_timer.getElapsedTime().asSeconds() / m_time.asSeconds()) * 0.5f + 0.25f;

		ExplosionEffect* e = sz::World::spawn<ExplosionEffect>(this);
		e->setScale(m_scale.x * t, m_scale.y * t);
		//e->setLayer(sz::EntityLayer::Foreground, 24);

		m_trailTimer.restart();
	}

	//!m_exploded && 
	if(m_timer.getElapsedTime() >= m_time)
	{
		auto pos = getTransform->getPosition();
		pos.y *= -1.f;
		getTransform->setPosition(pos);

		ExplosionEffect* e = sz::World::spawn<ExplosionEffect>(this);
		e->setScale(m_scale.x * 1.2f, m_scale.y * 1.2f);
		//e->setLayer(sz::EntityLayer::Foreground, 24);

		destroyEntity();

		m_exploded = true;
		//m_timer.restart();
	}
	/*else if(m_exploded && m_timer.getElapsedTime() >= sf::seconds(1.5f));
	{
		destroyEntity();
	}*/
}

////////////////////////////////////////////////////
void SecondaryExplosion::setScale(float min, float max)
{
	m_scale.x = min;
	m_scale.y = max;
}
