#include <game/Entities/Bullets/EnemyRotaryBullet.hpp>
#include <game/GameGlobals.hpp>

#include <game/Entities/Actors/PlayerEntity.hpp>

////////////////////////////////////////////////////
EnemyRotaryBullet::EnemyRotaryBullet()
{
	loadFromPrefab("data/bullets/enemy_rotary.szt");

	//setBeginContactCallback(&EnemyRotaryBullet::begincontact);

	m_collisionfilter.maskBits = 0x1 | 0x2;
	m_collisionfilter.categoryBits = 0x8;

	m_damage = 85;
}

////////////////////////////////////////////////////
EnemyRotaryBullet::~EnemyRotaryBullet()
{
	
}

////////////////////////////////////////////////////
void EnemyRotaryBullet::onLoad()
{
	//getComponent<sz::Transform>()->setScale(0.1f * 2.5f, 0.1f * 1.f);
}

////////////////////////////////////////////////////
bool EnemyRotaryBullet::onImpact(Entity* hit)
{
	auto impact = GameGlobals::get()->impact;
	auto bits = GameGlobals::get()->impactbits;

	auto position = getTransform->getPosition();
	auto color = getRenderer->getColor();

	impact->setColor(color);
	impact->setScale(0.35f, 0.45f + (thor::random(0, 10) >= 9 ? 0.6f : 0.f));
	impact->setPosition(position);
	impact->emit(1);
	
	bits->setColor(color);
	bits->setScale(0.05f, 0.12f);
	bits->setPosition(position);

	for(int i=0; i < 15; ++i)
	{
		float v = thor::random(0.f, 150.f);

		bits->setVelocityCone(v, m_angle-PI, thor::random(2.f, 46.f));
		bits->emit(1);
	}

	return true;
}

////////////////////////////////////////////////////
void EnemyRotaryBullet::derivedUpdate()
{
	float time = lifetimer.getElapsedTime().asSeconds();

	if(time > 5.5f)
	{
		auto impact = GameGlobals::get()->impact;
		auto position = getTransform->getPosition();
		auto color = getRenderer->getColor();

		impact->setColor(color);
		impact->setScale(0.35f, 0.45f + (thor::random(0, 10) >= 9 ? 0.6f : 0.f));
		impact->setPosition(position);
		impact->emit(1);

		destroyEntity();
		return;
	}

	if(m_speed < 85.f)
	{
		m_speed += std::max(0.f, time * time - 0.01f) * 15.f;
	}

	if(time <= 1.f)
	{
		auto player = GameGlobals::get()->player;
		float targetAngle = sz::getAngle(getTransform->getPosition(), player->getTransform->getPosition());

		m_angle += sz::angleDiff(m_angle, targetAngle) / 15.f;
	}
}
/*
////////////////////////////////////////////////////
void EnemyRotaryBullet::begincontact(sz::PhysicsContact& contact)
{
	destroyEntity();
}*/