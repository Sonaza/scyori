#include <szen/System/PausableClock.hpp>
#include <algorithm>

using namespace sz;

std::list<PausableClock*>		PausableClock::m_pausableClocks;

////////////////////////////////////////////////////
PausableClock::PausableClock() :
	m_paused(false)
{
	m_pausableClocks.push_back(this);
}

////////////////////////////////////////////////////
PausableClock::~PausableClock()
{
	m_pausableClocks.erase(
		std::remove_if(m_pausableClocks.begin(), m_pausableClocks.end(),
			[this](PausableClock* c) { return c == this; })
	);

	/*std::list<PausableClock*>::iterator it = std::find(m_pausableClocks.begin(), m_pausableClocks.end(), this);
	if(it != m_pausableClocks.end())
	{
		m_pausableClocks.erase(it);
	}*/
}

////////////////////////////////////////////////////
sf::Time PausableClock::getElapsedTime()
{
	sf::Time time = m_clock.getElapsedTime() - m_totalPauseTime;
	if(m_paused) time -= m_pauseTimer.getElapsedTime();

	return time;
}

////////////////////////////////////////////////////
sf::Time PausableClock::restart()
{
	sf::Time temp = getElapsedTime();

	m_clock.restart();
	m_pauseTimer.restart();

	m_totalPauseTime = sf::Time::Zero;

	return temp;
}

////////////////////////////////////////////////////
void PausableClock::resume()
{
	if(!m_paused) return;

	m_paused = false;
	m_totalPauseTime += m_pauseTimer.restart();
}

////////////////////////////////////////////////////
void PausableClock::pause()
{
	if(m_paused) return;

	m_paused = true;
	m_pauseTimer.restart();
}

////////////////////////////////////////////////////
void PausableClock::resumeAll()
{
	std::for_each(m_pausableClocks.begin(), m_pausableClocks.end(), [](PausableClock* c) { c->resume(); });
}

////////////////////////////////////////////////////
void PausableClock::pauseAll()
{
	std::for_each(m_pausableClocks.begin(), m_pausableClocks.end(), [](PausableClock* c) { c->pause(); });
}

////////////////////////////////////////////////////
bool PausableClock::isPaused() const
{
	return m_paused;
}