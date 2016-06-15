#ifndef SZEN_WINDOW_HPP
#define SZEN_WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

#include <cassert>

namespace sz
{
	
	class Window
	{
	public:

		// Opens window with set videomode, title, window style, antialiasing
		// and optionally virtual width which defaults to the same value as videomode
		static void open(sf::VideoMode videomode, const std::string &title, const sf::Uint32 style, const sf::Uint32 antialias, const sf::Uint32 virtualWidth = 0);
		static void close();
		
		static void changeMode(sf::VideoMode, const sf::Uint32 style);

		static void setVirtualWidth(const sf::Uint32 width);
		static sf::Vector2u getVirtualSize();

		static sf::Vector2u getSize();

		static sf::RenderWindow* getRenderWindow();
		
		static bool isActive();

		static sf::VideoMode getOptimalResolution(const bool fullscreen);
		static std::vector<sf::VideoMode> getSupportedResolutions(const bool fullscreen);
	private:

		// Use class only as a static entity
		Window(){}

	};
	
}

#endif // SZEN_WINDOW_HPP
