#include <game/Entities/Bullets/EnemyBossRedBullet.hpp>
#include <game/GameGlobals.hpp>

#include <game/Entities/Actors/PlayerEntity.hpp>

#include <game/Entities/Actors/Enemies/EnemyBossRed.hpp>

////////////////////////////////////////////////////
EnemyBossRedBullet::EnemyBossRedBullet()
{
	loadFromPrefab("data/bullets/boss_red_bullet.szt");

	//setBeginContactCallback(&EnemyBossRedBullet::begincontact);

	m_collisionfilter.maskBits = 0x1 | 0x2;
	m_collisionfilter.categoryBits = 0x8;

	m_damage = 330;
}

////////////////////////////////////////////////////
EnemyBossRedBullet::~EnemyBossRedBullet()
{
	
}

////////////////////////////////////////////////////
void EnemyBossRedBullet::onLoad()
{
	//getComponent<sz::Transform>()->setScale(0.1f * 2.5f, 0.1f * 1.f);
}

////////////////////////////////////////////////////
bool EnemyBossRedBullet::onImpact(Entity* hit)
{
	auto impact = GameGlobals::get()->impact;
	auto bits = GameGlobals::get()->impactbits;

	auto position = getTransform->getPosition();
	auto color = getRenderer->getColor();

	impact->setColor(color);
	impact->setScale(1.75f, 2.45f + (thor::random(0, 10) >= 9 ? 1.6f : 0.f));
	impact->setPosition(position);
	impact->emit(1);
	
	bits->setColor(color);
	bits->setScale(0.1f, 0.15f);
	bits->setPosition(position);

	for(int i=0; i < 35; ++i)
	{
		float v = thor::random(340.f, 800.f);

		bits->setVelocityCone(v, m_angle, 30.f);
		bits->emit(1);
	}

	sz::Camera::shake(0.5f);

	if(sz::World::exists(m_shooter))
	{
		m_bossPointer = dynamic_cast<EnemyBossRed*>(m_shooter);
		if(m_bossPointer)
		{
			m_damage = 330.f * ((m_bossPointer->getDamageModifier()-1.f) * 0.6f + 1.f);
		}
	}

	return true;
}

////////////////////////////////////////////////////
void EnemyBossRedBullet::derivedUpdate()
{
	float time = lifetimer.getElapsedTime().asSeconds();

	if(time > 2.5f)
	{
		auto impact = GameGlobals::get()->impact;
		auto position = getTransform->getPosition();
		auto color = getRenderer->getColor();

		impact->setColor(color);
		impact->setScale(1.75f, 2.45f + (thor::random(0, 10) >= 9 ? 1.6f : 0.f));
		impact->setPosition(position);
		impact->emit(1);

		destroyEntity();
		return;
	}

	if(m_speed < 150.f)
	{
		m_speed += std::max(0.f, time * time - 0.01f) * 7.f;
	}

	if(time <= 0.6f)
	{
		auto player = GameGlobals::get()->player;
		float targetAngle = sz::getAngle(getTransform->getPosition(), player->getTransform->getPosition());

		m_angle += sz::angleDiff(m_angle, targetAngle) / 25.f;
	}

	getPhysics->setRotation(sz::toDegrees(m_angle));
}
/*
////////////////////////////////////////////////////
void EnemyBossRedBullet::begincontact(sz::PhysicsContact& contact)
{
	destroyEntity();
}*/