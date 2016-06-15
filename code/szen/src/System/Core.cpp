#include <szen/System/Core.hpp>

#include <szen/System/AssetManager.hpp>
#include <szen/System/InputManager.hpp>
#include <szen/System/SceneManager.hpp>

#include <szen/Game/World.hpp>
#include <szen/Game/Camera.hpp>
#include <szen/System/SpriteBatch.hpp>

#include <szen/Game/ComponentType.hpp>

#ifdef SFML_SYSTEM_WINDOWS
	#include <windows.h>
#endif

using namespace sz;

bool Core::m_safeExit = true;
bool Core::m_running = true;

////////////////////////////////////////////////////
Core::Core(void)
{
}

////////////////////////////////////////////////////
Core::~Core(void)
{
	
}

#include <szen/System/AssetManager.hpp>
#include <szen/System/Assets/TextureAsset.hpp>

#include <szen/System/Config.hpp>

////////////////////////////////////////////////////
bool Core::initialize(const GameStartup::Settings &options)
{
	uint32 maxsize = sf::Texture::getMaximumSize();
	if(maxsize < 3500)
	{
		szerr << "Your GPU does not support large enough textures.\n\n"
			  << "Please update your graphics drivers and/or upgrade your GPU."
			  << ErrorStream::error;

		return false;
	}

	if(!sf::Shader::isAvailable())
	{
		szerr << "Your GPU does not support shaders (required GLSL version 120).\n\n"
			  << "Please update your graphics drivers and/or upgrade your GPU."
			  << ErrorStream::error;

		return false;
	}

	m_options = options;
	Window::open(
		m_options.videomode,
		m_options.windowTitle,
		m_options.windowMode,
		m_options.antialiasing,
		m_options.virtualWidth);

	auto res = Window::getSize();
	auto vres = Window::getVirtualSize();

	float relativeScale = res.x / 1920.f;

	m_window = Window::getRenderWindow();

	{
		auto tex = Asset::loadTexture("loading", "data/loading.png", true)->getAsset();

		sf::Sprite loading(*tex);
		loading.setPosition(res.x / 2.f, res.y * 0.75f);
		loading.setOrigin(750.f, 190.f);

		loading.setScale(relativeScale * 0.76f, relativeScale * 0.76f);

		m_window->clear(sf::Color::Black);
		m_window->draw(loading);
		m_window->display();
	}

	SceneManager::init(this);

	// Initialize the physics world
	World::init(m_options.gravity);

	SpriteBatch::init();

	hexgrid = Asset::loadTexture("hexgrid", "data/hexgrid.png", false)->getAsset();

	shaderi.loadFromFile("data/effects/scanlinevignette.frag", sf::Shader::Fragment);
	shaderi.setParameter("u_texture", sf::Shader::CurrentTexture);
	shaderi.setParameter("u_grid", *hexgrid);
	shaderi.setParameter("u_resolution", res.x, res.y);
	
	//rtex.create(res.x, res.y);
	windowtexture.create(res.x, res.y);

	oldres = res;

	m_window->setMouseCursorVisible(false);

	auto mouseTexture = Asset::loadTexture("mousecursor", "data/mousecursor.png", false)->getAsset();
	mouse.setTexture(*mouseTexture);
	mouse.setOrigin(38.f, 38.f);
	mouse.setScale(relativeScale, relativeScale);

	music.openFromFile("data/snd/scyori.ogg");
	music.setLoop(true);
	music.setVolume(Config::getMusicVolume());
	music.play();

	/////////////////////////////////

	/*{
		sf::Texture* blank = Asset::loadTexture("blank", "data/blank.png", false)->getAsset();
		sf::Sprite blspr(*blank);

		sf::Shader shader;
		shader.loadFromFile("data/effects/mana_forge_green.frag", sf::Shader::Fragment);

		shader.setParameter("u_time", 0.f);

		sf::RenderTexture rt;
		rt.create(750, 750);
		rt.clear(sf::Color::Transparent);

		float time = 0.f;
		for(int i=0; i < 100; ++i)
		{
			time = i / 25.f;
			shader.setParameter("u_time", time * 2.f);

			rt.clear(sf::Color::Transparent);
			rt.draw(blspr, &shader);
			rt.display();

			std::stringstream file;
			file << "mf/frame_" << i << ".png";

			rt.getTexture().copyToImage().saveToFile(file.str());
		}
	}

	quit();*/

	return true;
}

////////////////////////////////////////////////////
void Core::uninitialize()
{
	music.stop();

	// Free component type data
	ComponentType::releaseComponentTypes();

	// Uninitialize physics world (deleting all entities etc)
	World::uninit();

	// Free loaded assets
	Asset::uninit();
}

////////////////////////////////////////////////////
void Core::loop()
{
	//sf::sleep(sf::milliseconds(500));

	sf::Clock updateTimer;
	float accumulator = 0.f;
	float delta = 1.f / 60.f;

	//m_window->setVerticalSyncEnabled(true);

	while(m_running)
	{
		auto res = Window::getSize();
		if(oldres != res)
		{
			shaderi.setParameter("u_resolution", res.x, res.y);
			windowtexture.create(res.x, res.y);

			float relativeScale = res.x / 1920.f;
			mouse.setScale(relativeScale, relativeScale);

			oldres = res;
		}

		music.setVolume(Config::getMusicVolume());

		// Update SFML events
		events();

		// Update world and scenes
		/*if(updateTimer.getElapsedTime().asSeconds() >= 1.f / 60.f)
		{
			update();
			updateTimer.restart();
		}*/

		accumulator += std::min(0.1f, updateTimer.restart().asSeconds());

		while(accumulator >= delta)
		{
			update();
			accumulator -= delta;
		}

		// Do rendering
		render();

		/*size_t st = 16 - updateTimer.restart().asMilliseconds();
		if(st >= 6)
			sf::sleep(sf::milliseconds(st));*/
	}

	Window::close();
}

////////////////////////////////////////////////////
void Core::events()
{
	#ifdef SFML_SYSTEM_WINDOWS

		// Fix for windows taskbar lag in SFML
		MSG msg;
		while(PeekMessage(&msg, NULL, 0xc042, 0xc042, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	#endif

	sf::Event event;
	while(m_window->pollEvent(event))
	{
		if(Window::isActive())
		{
			// Update input events
			Input::updateInputs(event);

			// Scene's events
			SceneManager::handleEvents(event);

			// Parse global application inputs
			switch(event.type)
			{
			case sf::Event::Closed:
				m_running = false;
			break;
			case sf::Event::KeyPressed:
				if(m_safeExit && event.key.code == sf::Keyboard::Escape)
				{
					m_running = false;
				}
			break;
			}
		}
	}
}

////////////////////////////////////////////////////
void Core::toggleSafeExit(bool enabled)
{
	m_safeExit = enabled;
}

////////////////////////////////////////////////////
void Core::quit()
{
	m_running = false;
}

////////////////////////////////////////////////////
void Core::update()
{
	Asset::update();

	SceneManager::update();

	World::updateWorld();
}

#include <thor/Math.hpp>

////////////////////////////////////////////////////
void Core::render()
{
	//m_window->clear(sf::Color(200, 60, 120));
	m_window->clear(sf::Color::Black);

	if(!Scene::isPaused()) Camera::update();

	// Apply camera transformations
	m_window->setView(Camera::getCameraView());

	// Render world objects
	World::renderWorld();

	m_window->setView(m_window->getDefaultView());
	//m_window->setView(Camera::getInterfaceView());

	shaderi.setParameter("u_flicker", thor::random(0.f, 1.f));

	windowtexture.update(*m_window);
	sf::Sprite ts(windowtexture);
	m_window->draw(ts, &shaderi);

	// Render interface entities
	World::renderInterface();

	// Render misc items
	SceneManager::render();

	mouse.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_window)));
	m_window->draw(mouse);

	m_window->display();
}
