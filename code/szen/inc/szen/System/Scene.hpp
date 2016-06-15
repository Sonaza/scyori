#ifndef SZEN_SCENE_HPP
#define SZEN_SCENE_HPP

#include <SFML/Graphics.hpp>
#include <szen/System/PausableClock.hpp>

namespace sz
{
	
	class Scene
	{
		friend class SceneManager;

	public:
		Scene();
		virtual ~Scene();

		virtual void load() =0;
		virtual void unload() =0;

		virtual void events(const sf::Event& event) =0;
		virtual void update() =0;

		virtual void render() =0;

		static bool isPaused();
		static void togglePause(bool enabled);

	protected:

		void updateClocks();

		struct Clocks
		{
			Clocks() : total(0.f), delta(0.f) {}

			float total;
			float delta;

			sf::Time totalTime;
			sf::Time deltaTime;
		} Time;

		PausableClock	m_totalTimeClock;
		PausableClock	m_deltaTimeClock;

		struct ScreenInfo
		{
			ScreenInfo() : width(0), height(0) {}

			sf::Uint32	width;
			sf::Uint32	height;

			sf::Vector2f size;
			sf::Vector2f center;
		} Screen;

	private:

		static bool	m_isPaused;
		
	};
	
}

#endif // SZEN_SCENE_HPP
