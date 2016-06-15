#include <game/Entities/Bullets/PlayerBullet.hpp>
#include <game/GameGlobals.hpp>

#include <game/Entities/Actors/PlayerEntity.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

////////////////////////////////////////////////////
PlayerBullet::PlayerBullet()
{
	loadFromPrefab("data/bullets/playerbullet.szt");

	//setCollisionFilterCallback(&PlayerBullet::collisionCallback);

	m_collisionfilter.maskBits = 0x1 | 0x2;
	m_collisionfilter.categoryBits = 0x8;

	m_damage = 120;
}

////////////////////////////////////////////////////
PlayerBullet::~PlayerBullet()
{
	
}

////////////////////////////////////////////////////
void PlayerBullet::onLoad()
{
	PlayerEntity* player = GameGlobals::get()->player;
	assert(player);

	sf::Color color;

	bulletStance = player->getStance();

	switch(bulletStance)
	{
	case PlayerEntity::Stance::Agile:
		color = sf::Color(140, 255, 50);
		break;

	case PlayerEntity::Stance::Defensive:
		color = sf::Color(40, 120, 250);
		break;

	case PlayerEntity::Stance::Offensive:
		color = sf::Color(255, 80, 50);
		break;
	}

	getComponent<sz::Renderer>()->setColor(color);
}

#include <game/Entities/Actors/Enemies/EnemyBossRed.hpp>

////////////////////////////////////////////////////
bool PlayerBullet::onImpact(Entity* hit)
{
	auto impact = GameGlobals::get()->impact;

	auto position = getComponent<sz::Transform>()->getPosition();

	auto color = getComponent<sz::Renderer>()->getColor();

	impact->setColor(color);
	if(bulletStance == PlayerEntity::Stance::Offensive)
		impact->setScale(0.6f, 1.f + (thor::random(0, 10) >= 9 ? 1.5f : 0.f));
	else
		impact->setScale(0.3f, 0.6f + (thor::random(0, 10) >= 9 ? 0.6f : 0.f));

	impact->setPosition(position);
	impact->emit(1);

	auto bits = GameGlobals::get()->impactbits;

	bits->setColor(color);
	bits->setScale(0.02f, 0.1f);
	bits->setPosition(position);

	for(int i=0; i < 50; ++i)
	{
		float v = thor::random(0.f, 250.f);

		bits->setVelocityCone(v, m_angle + (thor::random(0, 2) == 0 ? -PI : 0.f), thor::random(0.f, 50.f));
		bits->emit(1);
	}

	if(bulletStance == PlayerEntity::Stance::Offensive)
	{
		auto close = getComponent<sz::Physics>()->queryRadius(150.f);

		for(auto it = close.begin(); it != close.end(); ++it)
		{
			auto enemy = dynamic_cast<EnemyEntityBase*>(*it);
			if(!enemy || enemy == hit || enemy->isBoss()) continue;

			auto enemyPos = enemy->call(&sz::Transform::getPosition);
			float dist = sz::distance(position, enemyPos) / 22500.f;
			dist = std::min(1.f, dist);
			float falloff = (1.f - (dist * dist)) * 0.65f;

			float angle = sz::getAngle(position, enemyPos);

			enemy->applyDamage(DamageReport(m_shooter, m_damage * falloff, angle)); 
		}
	}

	auto enemy = dynamic_cast<EnemyEntityBase*>(hit);

	if(enemy)
	{
		auto pos = enemy->getTransform->getPosition();

		if(!enemy->isBoss())
		{
			impact->setColor(sf::Color::White);
			impact->setScale(0.3f, 0.4f + (thor::random(0, 10) >= 9 ? 0.3f : 0.f));
		}
		else
		{
			impact->setColor(color);
			impact->setScale(1.f, 1.4f + (thor::random(0, 10) >= 9 ? 0.4f : 0.f));
		}

		impact->setPosition(pos + sf::Vector2f(thor::random(-30.f, 30.f), thor::random(-30.f, 30.f)));
		impact->emit(1);

		impact->setPosition(pos + sf::Vector2f(thor::random(-30.f, 30.f), thor::random(-30.f, 30.f)));
		impact->emit(1);
	}

	{
		auto boss = dynamic_cast<EnemyBossRed*>(hit);
		if(boss)
		{
			boss->increaseDmgMod(0.2f);
		}
	}

	return true;
}

////////////////////////////////////////////////////
void PlayerBullet::derivedUpdate()
{
	float time = lifetimer.getElapsedTime().asSeconds();
	float scale = std::max(0.25f, 0.6f - time * 5.f);

	getComponent<sz::Transform>()->setScale(scale, scale);

	if(time > 2.7f)
	{
		destroyEntity();
	}

	if(m_speed < 220.f)
		m_speed += std::max(0.f, time * time - 0.005f) * 25.f;
	//m_speed += (150.f - m_speed) * Time.delta / 3.f;
}
/*
////////////////////////////////////////////////////
bool PlayerBullet::collisionCallback(sz::PhysicsContact& contact)
{
	bool collide = m_shooter != contact.entityB;

	if(collide) collide = collide && dynamic_cast<PlayerBullet*>(contact.entityB) == NULL;

	return collide;
}
*/