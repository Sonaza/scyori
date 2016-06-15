#ifndef GAME_GAMEGLOBALS_HPP
#define GAME_GAMEGLOBALS_HPP

#include <szen/szen.hpp>
#include <memory>

#include <game/Entities/Effects/ExplosionEffect.hpp>
#include <game/Entities/Effects/SecondaryExplosion.hpp>

class GameGlobals;

typedef std::shared_ptr<GameGlobals> GameGlobalsPtr;

class PlayerEntity;
class GameMessage;

class GameGlobals
{
public:
	~GameGlobals();

	static GameGlobalsPtr get();

	/////////////////////////////////////////////
	// Player stuff

	PlayerEntity*		player;

	sz::ParticleEntity*		impact;
	sz::ParticleEntity*		impactbits;

	void addMessage(const std::string& str);
	GameMessage*			gameMessage;

	void setBossSpawned(const bool v);
	bool isBossSpawned();

private:

	bool m_isBossSpawned;

	GameGlobals();

};

#endif // GAME_GAMEGLOBALS_HPP
