#ifndef SZEN_PAUSABLECLOCK_HPP
#define SZEN_PAUSABLECLOCK_HPP

#include <SFML/System.hpp>
#include <list>

namespace sz
{
	
	class PausableClock
	{
	public:
		PausableClock();
		virtual ~PausableClock();

		sf::Time getElapsedTime();
		sf::Time restart();

		void resume();
		void pause();

		static void resumeAll();
		static void pauseAll();

		bool isPaused() const;

	private:

		// Stores whether clock is currently paused
		bool		m_paused;

		// Clock main timer
		sf::Clock	m_clock;
		// Calculates time paused
		sf::Clock	m_pauseTimer;

		// Total accumulated time paused
		sf::Time	m_totalPauseTime;

		// All currently existing pausableclocks
		static std::list<PausableClock*>	m_pausableClocks;

	};
	
}

#endif // SZEN_PAUSABLECLOCK_HPP
