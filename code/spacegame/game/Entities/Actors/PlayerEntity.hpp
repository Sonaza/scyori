#ifndef GAME_PLAYERENTITY_HPP
#define GAME_PLAYERENTITY_HPP

#include <szen/szen.hpp>
#include <game/Entities/Actors/ActorEntityBase.hpp>

class PlayerLash;
class PlayerCharge;
class PlayerFade;
class PlayerSmoke;

class EnergyBar;
class FluxBar;

typedef sz::Entity PlayerCannon;

class PlayerEntity : public ActorEntityBase
{
public:
	PlayerEntity();
	~PlayerEntity();

	void onLoad();
	void update();

	void begincontact(sz::PhysicsContact&);
	bool collisionCallback(sz::PhysicsContact&);

	void reset();
	void spawn();

	bool isSpawned();

	bool isDead() { return m_isDead; }

	sf::Time getDeathTimer() { return m_deathTimer.getElapsedTime(); }

private:

	bool m_isDead;

	sz::PausableClock	m_deathTimer;

	void spawnUpdate();

	sz::PausableClock	m_spawnTimer;
	int32				m_spawnPhase;

public:

	float	m_lastEnergy, m_currentEnergy;
	float	m_lastFlux, m_currentFlux;

	PlayerCannon*	m_cannon;

	EnergyBar*		m_energyBar;
	FluxBar*		m_fluxBar;

	PlayerSmoke*	m_smoke;

	struct Stance
	{
		typedef int Type;
		enum
		{
			Agile,
			Defensive,
			Offensive

		};
	};

	Stance::Type getStance() const { return m_stance; }

	float getEnergy() const { return m_energy.current; }
	float getEnergyMax() const { return m_energy.maximum; }

	void applyDamage(float amount);
	void applyRawDamage(float amount);
	void applyHeal(float amount);

	void setEnergy(float amount);

	void resetEnergy();
	void resetEnergy(float amount);

	// Returns the current energylevel normalized between 0-1
	float getEnergyFraction() { return m_energy.current / m_energy.maximum; }
	
	float getDamageModifier();

	float getEnergyPercentage(float p) { return m_energy.maximum * p * 0.01f; }

	/////////////////////////

	float getFlux() { return m_flux.current; }
	float getFluxMax() { return m_flux.maximum; }

	void setFlux(float amount);

	void resetFlux();
	void resetFlux(float amount);

	void addFlux(float amount);
	void removeFlux(float amount);

	float getFluxFraction() { return m_flux.current / m_flux.maximum; }
	float getFluxPercentage(float p) { return m_flux.maximum * p * 0.01f; }

	////////////////////////////////////////////
	// Player ability methods & variables

	// Agile stuff
	PlayerLash*		m_abilityLash;
	PlayerCharge*	m_abilityCharge;

	// Fade entity
	PlayerFade*		m_abilityFade;

	// Cooldowns
	struct Cooldowns
	{

		sf::Time	cannon;
		sf::Time	stance_change;

	} m_cooldowns;

	sz::Entity*	m_glow;

private:

	////////////////////////////
	// SOUNDS
	
	sz::SoundAsset* snd_cannon;
	sz::SoundAsset* snd_stance_charge;
	sz::SoundAsset* snd_stance_hit;

	////////////////////////////


	b2Vec2 vel;

	void miscUpdate();

	void arealimiter();

	void cameraUpdate();

	void handleInput();

	void handleMovement();
	void handleAbilities();

	void stanceUpdate();

	sz::PausableClock	m_cannonTimer;

	////////////////////////////////////////////
	// Health mechanic

	struct PlayerResources
	{
		float	current;
		float	maximum;
	};

	PlayerResources		m_energy;
	PlayerResources		m_flux;

	////////////////////////////////////////////
	// Stance methods and variables

	void switchStance(const Stance::Type stance);

	Stance::Type		m_stance;
	sz::PausableClock	m_stanceTimer;

	float				m_stanceFlux;

	bool				m_stanceCharging;
	float				m_stanceChargePhase;

	bool				m_stanceKeyReset;

	sz::Entity*			m_stanceWave;

};

#endif // GAME_PLAYERENTITY_HPP
