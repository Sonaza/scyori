#ifndef SZEN_INPUTMANAGER_HPP
#define SZEN_INPUTMANAGER_HPP

#include <sfml/Graphics.hpp>
#include <map>

namespace sz
{

	typedef std::map<const sf::Keyboard::Key, int> KeyHits;
	typedef std::map<const sf::Mouse::Button, int> MouseHits;
	
	namespace Input
	{

		bool isKeyDown(const sf::Keyboard::Key code);
		bool isKeyHit(const sf::Keyboard::Key code);

		bool isMouseDown(const sf::Mouse::Button button);
		bool isMouseHit(const sf::Mouse::Button button);

		sf::Vector2i getMousePos();
		sf::Vector2i getMouseWorldPos();

		void updateInputs(sf::Event& event);

		void clearInputs();

	}
	
}

#endif // SZEN_INPUTMANAGER_HPP
