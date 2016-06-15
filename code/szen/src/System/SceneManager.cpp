#include <szen/System/SceneManager.hpp>
#include <szen/System/AssetManager.hpp>

#include <szen/System/Window.hpp>

using namespace sz;

namespace
{
	ScenePtr	m_currentScene;
	Core*		p_core;

	ScenePtr	m_overlayScene;

	bool		m_toggle = false;
}

////////////////////////////////////////////////////
void SceneManager::init(Core* corePtr)
{
	p_core = corePtr;
}

////////////////////////////////////////////////////
void SceneManager::setScene(Scene* scene)
{
	assert(scene != NULL && "Scene can't be null");

	// Unload previous scene before changing to the next
	if(m_currentScene)
	{
		Asset::freeSceneAssets();

		m_currentScene->unload();
	}

	// Replace the current pointer
	m_currentScene.reset(scene);

	// Set screen size
	sf::Vector2u size		= Window::getSize();
	scene->Screen.width		= size.x;
	scene->Screen.height	= size.y;
	scene->Screen.size		= static_cast<sf::Vector2f>(size);
	scene->Screen.center	= scene->Screen.size / 2.f;

	// Load the new scene's content
	scene->load();
}

////////////////////////////////////////////////////
void SceneManager::setOverlayScene(Scene* scene)
{
	assert(scene != NULL && "Scene can't be null");

	// Unload previous scene before changing to the next
	if(m_overlayScene)
	{
		m_overlayScene->unload();
	}

	// Replace the current pointer
	m_overlayScene.reset(scene);

	// Set screen size
	sf::Vector2u size		= Window::getSize();
	scene->Screen.width		= size.x;
	scene->Screen.height	= size.y;
	scene->Screen.size		= static_cast<sf::Vector2f>(size);
	scene->Screen.center	= scene->Screen.size / 2.f;

	// Load the new scene's content
	scene->load();
}

////////////////////////////////////////////////////
void SceneManager::handleEvents(const sf::Event &event)
{
	assert(m_currentScene && "Scene must be initialized");

	m_currentScene->events(event);

	if(m_overlayScene && m_toggle)
	{
		m_overlayScene->events(event);
	}
}

////////////////////////////////////////////////////
void SceneManager::update()
{
	assert(m_currentScene && "Scene must be initialized");

	m_currentScene->updateClocks();
	m_currentScene->update();

	if(m_overlayScene && m_toggle)
	{
		m_overlayScene->updateClocks();
		m_overlayScene->update();
	}
}

////////////////////////////////////////////////////
void SceneManager::render()
{
	assert(m_currentScene && "Scene must be initialized");

	m_currentScene->render();

	if(m_overlayScene && m_toggle)
	{
		m_overlayScene->render();
	}
}

////////////////////////////////////////////////////
bool SceneManager::isOverlayEnabled()
{
	return m_toggle;
}

////////////////////////////////////////////////////
void SceneManager::toggleOverlay(bool enabled)
{
	m_toggle = enabled;
}