#include <game/Entities/Bullets/EnemyHeavyBullet.hpp>
#include <game/GameGlobals.hpp>

////////////////////////////////////////////////////
EnemyHeavyBullet::EnemyHeavyBullet()
{
	loadFromPrefab("data/bullets/enemy_heavy.szt");

	//setBeginContactCallback(&EnemyHeavyBullet::begincontact);

	m_collisionfilter.maskBits = 0x1 | 0x2;
	m_collisionfilter.categoryBits = 0x8;

	m_damage = 65;
}

////////////////////////////////////////////////////
EnemyHeavyBullet::~EnemyHeavyBullet()
{
	
}

////////////////////////////////////////////////////
void EnemyHeavyBullet::onLoad()
{
	//getComponent<sz::Transform>()->setScale(0.1f * 2.5f, 0.1f * 1.f);
}

////////////////////////////////////////////////////
bool EnemyHeavyBullet::onImpact(Entity* hit)
{
	auto impact = GameGlobals::get()->impact;
	auto bits = GameGlobals::get()->impactbits;

	auto position = getTransform->getPosition();
	auto color = getRenderer->getColor();

	impact->setColor(color);
	impact->setScale(0.2f, 0.35f + (thor::random(0, 10) >= 9 ? 0.6f : 0.f));
	impact->setPosition(position);
	impact->emit(1);
	
	bits->setColor(color);
	bits->setScale(0.02f, 0.1f);
	bits->setPosition(position);

	for(int i=0; i < 10; ++i)
	{
		float v = thor::random(0.f, 120.f);

		bits->setVelocityCone(v, m_angle-PI, thor::random(2.f, 60.f));
		bits->emit(1);
	}

	return true;
}

////////////////////////////////////////////////////
void EnemyHeavyBullet::derivedUpdate()
{
	float time = lifetimer.getElapsedTime().asSeconds();

	if(time > 2.7f)
	{
		destroyEntity();
	}

	m_speed += std::max(0.f, time * time - 0.01f) * 25.f;
}
/*
////////////////////////////////////////////////////
void EnemyHeavyBullet::begincontact(sz::PhysicsContact& contact)
{
	destroyEntity();
}*/