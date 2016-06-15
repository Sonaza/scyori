#ifndef SZEN_CORE_HPP
#define SZEN_CORE_HPP

#include <sfml/Graphics.hpp>
#include <sfml/Audio.hpp>

#include <szen/GameStartup.hpp>
#include <szen/System/Window.hpp>

#include <string>
#include <memory>
#include <iostream>

namespace sz
{

	class Core
	{
	public:
		Core(void);
		~Core(void);

		bool initialize(const GameStartup::Settings &options);
		void uninitialize();

		void loop();

		void events();
		void update();

		void render();
		
		static void quit();
		static void toggleSafeExit(bool);

	protected:

		static bool	m_safeExit;

		sf::Music	music;

		sf::Sprite	mouse;

		sf::Shader shaderi;
		sf::Texture windowtexture;
		sf::Texture* hexgrid;

		sf::Vector2u oldres;

		////////////////////////////////////////////////////////
		// Member variables

		// if application is running
		static bool	m_running;

		GameStartup::Settings m_options;

		sf::RenderWindow* m_window;
		
	};

}

#endif