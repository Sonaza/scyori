#include <szen/System/Scene.hpp>

#include <szen/System/Window.hpp>
#include <szen/System/PausableClock.hpp>

using namespace sz;

////////////////////////////////////////////////////
Scene::Scene()
{
	m_deltaTimeClock.restart();
	m_totalTimeClock.restart();
}

////////////////////////////////////////////////////
Scene::~Scene()
{
}

////////////////////////////////////////////////////
void Scene::updateClocks()
{
	Time.deltaTime	= m_deltaTimeClock.restart();
	Time.delta		= Time.deltaTime.asSeconds();

	Time.totalTime	= m_totalTimeClock.getElapsedTime();
	Time.total		= Time.totalTime.asSeconds();

	sf::Vector2u size	= Window::getSize();
	Screen.width		= size.x;
	Screen.height		= size.y;
	Screen.size			= static_cast<sf::Vector2f>(size);
	Screen.center		= Screen.size / 2.f;
}

bool Scene::m_isPaused = false;

////////////////////////////////////////////////////
bool Scene::isPaused()
{
	return m_isPaused;
}

////////////////////////////////////////////////////
void Scene::togglePause(bool enabled)
{
	m_isPaused = enabled;
	if(enabled)
	{
		PausableClock::pauseAll();
	}
	else
	{
		PausableClock::resumeAll();
	}
}