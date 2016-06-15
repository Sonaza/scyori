#include <game/Entities/Abilities/PlayerLash.hpp>
#include <game/Entities/Actors/Enemies/EnemyEntityBase.hpp>

////////////////////////////////////////////////////
PlayerLash::PlayerLash(PlayerEntity* player) :
	p_player		(player),
	m_isLashing		(false),
	m_rotation		(0.f),
	m_targetRotation(0.f),
	m_rotationStep	(0.f),
	m_checkInterval	(0.f),
	m_scale			(0.f),
	m_targetScale	(0.f),
	m_scaleStep		(0.f)
{
	loadFromPrefab("data/effects/player_lash.szt");

	m_rotations.resize(23);

	int i = 0;
	m_rotations[i++] = -180.f;
	m_rotations[i++] = -177.2041f;
	m_rotations[i++] = -168.5363f;
	m_rotations[i++] = -153.7281f;
	m_rotations[i++] = -132.7766f;
	m_rotations[i++] = -105.9901f;
	m_rotations[i++] = -73.9754f;
	m_rotations[i++] = -37.5615f;
	m_rotations[i++] = 2.3147f;
	m_rotations[i++] = 44.721f;
	m_rotations[i++] = 88.816f;
	m_rotations[i++] = 133.889f;
	m_rotations[i++] = 179.379f;
	m_rotations[i++] = 224.854f;
	m_rotations[i++] = 270.f;
	m_rotations[i++] = 294.71f;
	m_rotations[i++] = 309.299f;
	m_rotations[i++] = 320.743f;
	m_rotations[i++] = 330.371f;
	m_rotations[i++] = 338.701f;
	m_rotations[i++] = 345.958f;
	m_rotations[i++] = 352.191f;
	m_rotations[i++] = 357.227f;

	m_scales.resize(23);

	i = 0;
	m_scales[i++] = 0.319606f * 0.1f;
	m_scales[i++] = 0.627232f * 0.35f;
	m_scales[i++] = 0.798552f * 0.5f;
	m_scales[i++] = 0.899468f * 0.7f;
	m_scales[i++] = 0.958545f * 0.9f;
	m_scales[i++] = 0.989502f;
	m_scales[i++] = 1.f;
	m_scales[i++] = 1.f;
	m_scales[i++] = 1.f;
	m_scales[i++] = 1.f;
	m_scales[i++] = 1.f;
	m_scales[i++] = 1.f;
	m_scales[i++] = 0.957546f;
	m_scales[i++] = 0.881436f;
	m_scales[i++] = 0.792893f;
	m_scales[i++] = 0.698018f;
	m_scales[i++] = 0.59976f;
	m_scales[i++] = 0.5f;
	m_scales[i++] = 0.40024f;
	m_scales[i++] = 0.301982f;
	m_scales[i++] = 0.207107f;
	m_scales[i++] = 0.118564f;
	m_scales[i++] = 0.0424537f;

}

////////////////////////////////////////////////////
PlayerLash::~PlayerLash()
{
	
}

////////////////////////////////////////////////////
bool PlayerLash::isLashing()
{
	return m_isLashing;
}

////////////////////////////////////////////////////
void PlayerLash::update()
{
	getComponent<sz::Transform>()->setScale(sf::Vector2f(m_scale, m_scale));

	sz::Animation* animation = getComponent<sz::Animation>();
	if(m_isLashing && animation->isPlaying())
	{
		float prevrot = m_rotation;

		float delta = m_rotationStep * Time.delta;
		m_rotation += delta;
		m_checkInterval += delta;

		m_scale += m_scaleStep * Time.delta;

		getComponent<sz::Transform>()->setRotation(m_rotation);
		getComponent<sz::Transform>()->setScale(sf::Vector2f(m_scale, m_scale) * 1.7f);

		m_progress += animation->getCurrentFPS() / 23.f * Time.delta;

		sf::Vector2f myPosition = getComponent<sz::Transform>()->getPosition();

		if(m_frame < 22)
		{
			auto ib = GameGlobals::get()->impactbits;

			for(float a = prevrot; a <= m_rotation; a += 1.f)
			{
				float rotrad = sz::toRadians(a-5.f);
				sf::Vector2f dir(
					cos(rotrad), sin(rotrad)
				);

				ib->setScale(0.06f, 0.12f);
				ib->setColor(sf::Color(180, 100, 30));

				const int nbparticles = (int)((m_scale * 3.f + m_progress * 5.f * m_scale) * 0.5f);
				//thor::random(1, 2);
				for(int i=0; i < nbparticles; ++i)
				{
					//float len = thor::random(540.f - 100.f * speed, 580.f) * speed;
					float x = thor::random(8.f - m_scale * 5.f, 10.f);
					float len = (10.f * (x + 10.f) - (x-5.f)*(x-5.f)) / 175.f * 560.f * m_scale;
					ib->setPosition(myPosition + dir * len);

					float v = thor::random(-200.f, 0.f);

					ib->setVelocityCone(v, rotrad - thor::Pi * 0.5f, 1.f);//thor::random(1.f, 30.f));
					ib->emit(1);
				}
			}
		}

		////////////////////////

		int32 frame = animation->getCurrentFrame()-1;

		if(frame != m_frame)
		{
			m_frame = frame;

			m_rotation			= m_rotations[frame];
			m_targetRotation	= frame < 22 ? m_rotations[frame+1] : m_rotation;

			m_rotationStep		= (m_targetRotation - m_rotation) * animation->getCurrentFPS();

			//m_scale			= m_scales[frame];
			m_targetScale	= frame < 22 ? m_scales[frame+1] : m_scale;

			m_scaleStep		= (m_targetScale - m_scale) * animation->getCurrentFPS();
		}

		if(m_checkInterval >= 6.f && m_frame >= 1 && m_frame < 22)
		{
			m_checkInterval = 0.f;

			if(m_enemyList.empty())
			{
				//m_enemyList = p_player->getPhysics->queryRadius(500.f * m_scale);
			}

			bool iterated = false;

			std::vector<Entity*> enemies = p_player->getPhysics->queryRadius(500.f * m_scale);
			for(std::vector<Entity*>::iterator it = enemies.begin(); it != enemies.end(); ++it)
			{
				//break;
				//iterated = false;

				EnemyEntityBase* e = dynamic_cast<EnemyEntityBase*>(*it);
				if(e == NULL) continue;
				if(!sz::World::exists(e))
				{
					//it = m_enemyList.erase(it);
					continue;
				}

				sf::Vector2f position = e->getComponent<sz::Transform>()->getPosition();

				float angle = sz::toDegrees(sz::getAngle(myPosition, position));
				float diff = fabs(sz::angleDiffDeg(m_rotation, angle));
				if(diff <= 12.f)
				{
					float damageAngle = sz::toRadians(m_rotation + 120.f);
					if(e->applyDamage(DamageReport(p_player, 300.f * p_player->getDamageModifier(), damageAngle)))
					{
						sz::Camera::shake(2.f);
						//it = m_enemyList.erase(it);
						//iterated = true;
					}

					auto impact = GameGlobals::get()->impact;

					impact->setColor(sf::Color(255, 200, 90));
					impact->setScale(0.7f, 1.25f);
					impact->setPosition(position); 
					impact->emit(1);

					auto ib = GameGlobals::get()->impactbits;

					ib->setPosition(position);
					ib->setColor(sf::Color(255, 200, 100));
					ib->setScale(0.02f, 0.08f);

					for(int i=0; i < 8; ++i)
					{
						float v = thor::random(50.f, 150.f);

						ib->setVelocityCone(v, damageAngle, thor::random(2.f, 30.f));
						ib->emit(1);
					}

					//e->getComponent<sz::Physics>()->accelerate(sz::toRadians(m_rotation+30.f), 250.f);

				}

				// Iterator stuff
				//if(!iterated) ++it;
			}
		}
	}
	else
	{
		m_isLashing = false;
		//m_enemyList.clear();
	}
}

////////////////////////////////////////////////////
void PlayerLash::activate()
{
	if(m_isLashing) return;
	m_isLashing = true;

	m_enemyList.clear();

	sz::Animation* animation = getComponent<sz::Animation>();
	animation->play("cleave");

	m_frame				= 0;

	m_progress			= 0.f;

	m_checkInterval		= 0.f;

	m_rotation			= m_rotations[0];
	m_targetRotation	= m_rotations[1];
	m_rotationStep		= (m_targetRotation - m_rotation) / animation->getCurrentFPS();

	getComponent<sz::Transform>()->setRotation(m_rotation);
}