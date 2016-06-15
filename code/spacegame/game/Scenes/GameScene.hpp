#ifndef GAME_GAMESCENE_HPP
#define GAME_GAMESCENE_HPP

#include <szen/szen.hpp>
#include <iostream>
#include <cmath>

#include <sstream>

#include <game/Entities/Actors/PlayerEntity.hpp>

#include <game/Entities/Actors/Enemies/EnemyBasic.hpp>
#include <game/Entities/Actors/Enemies/EnemyHeavy.hpp>
#include <game/Entities/Actors/Enemies/EnemyHugger.hpp>
#include <game/Entities/Actors/Enemies/EnemyRotary.hpp>

#include <game/Entities/Misc/GameMessage.hpp>

#include <Thor/Math.hpp>

#include <memory>

class GameScene : public sz::Scene
{
public:
	GameScene();
	~GameScene();

	void load();
	void unload();

	void stationLoad();

	void events(const sf::Event& event);
	void update();

	void render();

	///////////////////
	// Game things

	void startGame();
	void resetGame();

	void pauseGame();

	void stopGame();

	bool isGameStarted();

	void setSpawnSettings(EnemyEntityBase* enemy, sf::Vector2f pos);

	template <class Type>
	Type* spawnEnemy(sf::Vector2f pos)
	{
		Type* enemy = sz::World::spawn<Type>();
		sf::Vector2f offset(thor::random(-50.f, 50.f), thor::random(-50.f, 50.f));
		enemy->getPhysics->setPosition(pos+offset);
		//setSpawnSettings(enemy, pos);

		return enemy;
	}

	sz::Entity*		m_gameoverScreen;

private:

	sz::PausableClock	m_gameTimer;

	sz::Entity*		m_stationAnchor;
	sz::Entity*		m_stationWallAnchor;

	void	spawnUpdate();

	int32	m_currentWave;

	bool	m_gameStarted;

	///////////////////////////////////////////////
	// Player variables

	PlayerEntity*		m_player;

	///////////////////////////////////////////////
	// Game logic variables

	sz::PausableClock	m_enemySpawnTimer;

	float fps;
	sf::Clock fpsclock, fpsupdate;

	ActorEntityBase* test2;

	sz::ParticleEntity* impactparticles;

	float m_zoom;

};

#endif GAME_TESTSCENE_HPP
