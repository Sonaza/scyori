#include <game/Game.hpp>
#include <game/Scenes/GameScene.hpp>

#include <szen/System/Config.hpp>

////////////////////////////////////////////////////
Game::Game(void)
{
}

////////////////////////////////////////////////////
Game::~Game(void)
{
}

////////////////////////////////////////////////////
void Game::launch()
{
	std::unique_ptr<Game> game(new Game);
	if(game) game->run();
}

////////////////////////////////////////////////////
void Game::initOptions()
{
	Options.windowMode		= sz::Config::getSystemWindowMode();
	
	Options.videomode		= sz::Config::getVideoMode();

	Options.virtualWidth	= 1920;

	Options.windowTitle		= "Scyori";
	Options.antialiasing	= 2;
}

////////////////////////////////////////////////////
void Game::startup()
{
	// Set up the startup scene
	createScene<GameScene>();
}
