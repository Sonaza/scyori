#ifndef GAME_ENEMYENTITYBASE_HPP
#define GAME_ENEMYENTITYBASE_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/ActorEntityBase.hpp>
#include <game/Entities/Bullets/BulletEntityBase.hpp>

#include <game/Entities/Actors/PlayerEntity.hpp>

#include <game/Misc/DamageReport.hpp>

class EnemyEntityBase : public ActorEntityBase
{
public:
	EnemyEntityBase();
	~EnemyEntityBase();

	virtual void updateAI() = 0;

	void internalUpdate();

	void updateMovement(float mindistance, float maxdistance, float speed, float radius = 60.f);

	void begincontact(sz::PhysicsContact&);
	void endcontact(sz::PhysicsContact&);

	bool collisionCallback(sz::PhysicsContact&);

	//virtual void onBirth();
	//virtual void onDeath(Entity* killer, Entity* bullet) =0;
	virtual void onDeath(DamageReport&) =0;

	bool applyDamage(DamageReport&);

	void spawnFlux(int32 amount);

	bool isBoss();
	void setAsBoss(bool toggle);

	float getHealthFraction() { return m_health.current / m_health.max; }

	void setVulnerability(float v) { m_vulnerabilityModifier = v; }
	float getVulnerability() { return m_vulnerabilityModifier; }

protected:

	float m_vulnerabilityModifier;

	bool m_isBoss;

	PlayerEntity*	p_player;

	struct Health
	{
		Health() : current(0.f), max(0.f) {}

		float current;
		float max;

		void set(float value)
		{
			current = max = value;
		}

	} m_health;

	void resetBrainfart();

private:

	std::vector<float>	m_strafeMod;
	bool				m_isStrafing;
	sz::PausableClock	m_strafeTimer;
	sf::Time			m_strafeDelay;

	sz::PausableClock	m_brainfart;

	float	m_flashPhase;

	void kill(DamageReport& deathdata);
	bool checkDeathStatus();

};

#endif // GAME_ENEMYENTITYBASE_HPP
