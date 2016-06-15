#include <szen/GameStartup.hpp>

#include <szen/System/Core.hpp>
#include <szen/System/SceneManager.hpp>

#include <szen/System/Config.hpp>

using namespace sz;

////////////////////////////////////////////////////
GameStartup::GameStartup()
{
	
}

////////////////////////////////////////////////////
GameStartup::~GameStartup()
{
	
}

////////////////////////////////////////////////////
GameStartup::Settings::Settings() :
	videomode		(1280, 720),
	virtualWidth	(1280),
	windowTitle		("Szen window"),
	windowMode		(sf::Style::Close),
	antialiasing	(0),
	gravity			(0.f, 0.f)
{
}

////////////////////////////////////////////////////
void GameStartup::run()
{
	m_core = CorePtr(new Core);

	Config::initSettingsFile();

	// Initialize options
	initOptions();

	// Initializing
	if(m_core->initialize(Options))
	{
		// Call inherited startup for options
		startup();

		// Enter the main loop
		m_core->loop();

		// Main loop has been exited and it's time to say goodbye
		m_core->uninitialize();
	}

	Config::closeSettings();
}

////////////////////////////////////////////////////
void GameStartup::createScene(Scene* scene)
{
	assert(scene && "Scene can't be null");
	SceneManager::setScene(scene);
}
