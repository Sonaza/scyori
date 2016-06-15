#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

#include <game/Entities/Effects/Fluxicle.hpp>

////////////////////////////////////////////////////
EnemyEntityBase::EnemyEntityBase() :
	p_player		(NULL),
	m_flashPhase	(0.f),
	m_isBoss		(false)
{
	m_type = Enemy;

	setBeginContactCallback(&EnemyEntityBase::begincontact);

	p_player = dynamic_cast<PlayerEntity*>(p_gameGlobals->player);

	m_strafeMod.resize(10, 0.f);

	int i=0;
	m_strafeMod[i++] = thor::random(0, 1) ? 1.f : -1.f;
	m_strafeMod[i++] = thor::random(0.1f, 1.f);
	m_strafeMod[i++] = thor::random(2.f, 4.5f);
	m_strafeMod[i++] = thor::random(0, 1) ? 1.f : -1.f;

	m_strafeMod[i++] = thor::random(0.4f, 1.2f);

	m_isStrafing = false;
	m_strafeDelay = sf::seconds(0.5f);

	m_vulnerabilityModifier = 1.f;
}

////////////////////////////////////////////////////
EnemyEntityBase::~EnemyEntityBase()
{
	
}

////////////////////////////////////////////////////
bool EnemyEntityBase::isBoss()
{
	return m_isBoss;
}

////////////////////////////////////////////////////
void EnemyEntityBase::setAsBoss(bool toggle)
{
	m_isBoss = toggle;
}

////////////////////////////////////////////////////
void EnemyEntityBase::spawnFlux(int32 amount)
{
	if(amount <= 0) return;

	for(int i=0; i < amount; ++i)
	{
		sz::World::spawn<Fluxicle>(this);
	}
}

////////////////////////////////////////////////////
bool EnemyEntityBase::applyDamage(DamageReport& dr)
{
	m_health.current -= dr.getDamage() * m_vulnerabilityModifier;
	if(checkDeathStatus())
	{
		kill(dr);

		return true;
	}

	auto ib = GameGlobals::get()->impactbits;

	ib->setPosition(getComponent<sz::Transform>()->getPosition());

	if(dr.getBullet() != NULL)
	{
		ib->setColor(dr.getBullet()->getComponent<sz::Renderer>()->getColor());
	}
	else
	{
		ib->setColor(sf::Color(255, 200, 100));
	}

	for(int i=0; i < 5; ++i)
	{
		float v = thor::random(20.f, 150.f);

		ib->setVelocityCone(v, 0.f, 180.f);
		ib->emit(1);
	}

	m_flashPhase = 1.f;

	return false;
}

////////////////////////////////////////////////////
void EnemyEntityBase::internalUpdate()
{
	if(!m_isBoss)	m_flashPhase += (0.f - m_flashPhase) / 8.f;
	else			m_flashPhase += (0.f - m_flashPhase) / 4.f;
	
	getComponent<sz::Renderer>()->setColor(
		sf::Color((uint8)(m_flashPhase * 255.f * (m_isBoss ? 0.4f : 1.f)), 255, 255));
}

////////////////////////////////////////////////////
void EnemyEntityBase::updateMovement(float mindistance, float maxdistance, float speed, float radius)
{
	mindistance *= mindistance;
	maxdistance *= maxdistance;

	//PlayerEntity* player = dynamic_cast<PlayerEntity*>(p_gameGlobals->player);
	assert(p_player);

	sf::Vector2f playerPosition = p_player->call(&sz::Transform::getPosition);

	sz::Transform* transform = getComponent<sz::Transform>();
	sz::Physics* physics = getComponent<sz::Physics>();

	sf::Vector2f myPosition = transform->getPosition();

	float angle = sz::getAngle(myPosition, playerPosition);
	physics->setRotation(sz::toDegrees(angle));

	///////////

	float distance = sz::distance(myPosition, playerPosition);

	float forward = 1.f;

	float myangle = sz::toRadians(transform->getRotation());

	if((m_isStrafing || distance <= 2240000.f) && m_strafeTimer.getElapsedTime() >= m_strafeDelay)
	{
		if(!m_isStrafing)
		{
			m_strafeDelay = sf::milliseconds(thor::random(1500, 4200));
		}
		else
		{
			m_strafeDelay = sf::milliseconds(thor::random(500, 2700));
		}

		m_strafeTimer.restart();

		m_isStrafing = !m_isStrafing;
	}

	if(m_isStrafing)
	{
		float x = Time.total;
		float strafeIntensity = 
			(cos(x * 0.7f) + cos(-x * 0.5f - 3.1f) * m_strafeMod[0]) *
			(sin(x * m_strafeMod[1]) * m_strafeMod[3] + cos(x + sin(x * m_strafeMod[2]) * 2.f - 2.f));

		physics->accelerate(myangle + PI * 0.5f, strafeIntensity);
	}

	if(radius > 0.f)
	{
		std::vector<Entity*> friends = physics->queryRadius(radius, 0);

		ActorEntityBase* other = NULL;
		for(std::vector<Entity*>::iterator it = friends.begin(); it != friends.end(); ++it)
		{
			other = dynamic_cast<ActorEntityBase*>(*it);
			if(other == NULL || other == this) continue;

			sf::Vector2f otherPos = other->getComponent<sz::Transform>()->getPosition();

			if(myPosition == otherPos) continue;

			float angle = sz::getAngle(myPosition, otherPos);

			//float diff = fabs(angle - myangle + 3.141592f / 2.f);
			//if(diff > 3.141592f) continue;

			physics->accelerate(angle, -0.7f);

			forward *= 0.5f;
		}
	}

	if(forward >= 0.01f && distance > maxdistance)
	{
		physics->accelerate(angle, 0.25f * forward * speed);
	}
	else if(distance < mindistance && m_brainfart.getElapsedTime() >= sf::seconds(2.f))
	{
		physics->accelerate(angle, -0.5f * speed);
	}
	else
	{
		physics->brake(0.1f);
	}
}

////////////////////////////////////////////////////
void EnemyEntityBase::resetBrainfart()
{
	m_brainfart.restart();
}

////////////////////////////////////////////////////
bool EnemyEntityBase::checkDeathStatus()
{
	return m_health.current <= 0 && !isDestroyed();
}

////////////////////////////////////////////////////
void EnemyEntityBase::kill(DamageReport& dr)
{
	onDeath(dr);
	destroyEntity();
}

////////////////////////////////////////////////////
void EnemyEntityBase::begincontact(sz::PhysicsContact& contact)
{
	BulletEntityBase* bullet = dynamic_cast<BulletEntityBase*>(contact.entityB);
	if(bullet != NULL && bullet->m_shooter != this)
	{
		DamageReport dr(bullet->m_shooter, bullet);
		applyDamage(dr);
	}
}

////////////////////////////////////////////////////
void EnemyEntityBase::endcontact(sz::PhysicsContact&)
{

}

////////////////////////////////////////////////////
bool EnemyEntityBase::collisionCallback(sz::PhysicsContact&)
{
	return true;
}