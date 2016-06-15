#ifndef SZEN_CAMERA_HPP
#define SZEN_CAMERA_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

namespace sz
{
	
	namespace Camera
	{

		void updateScreenSize();

		// Set camera position
		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f &v);

		void shake(float intensity);
		void update();

		sf::Vector2f getPosition();

		// Translate camera on x and y axis
		void translate(float x, float y);
		void translate(const sf::Vector2f &v);

		// Moves camera according to the current rotation + optional offset
		void move(float distance, float angleOffset = 0.f);

		// Set's camera rotation to N degrees
		void setRotation(float a);
		// Returns camera's current rotation in degrees
		float getRotation();

		// Turns camera by N degrees
		void turn(float a);
		
		// Set camera zoom, default zoom level is 1.f
		// > 1.f = bigger
		// < 1.f = smaller
		void setZoom(float zoom);
		float getZoom();

		// Zoom camera with a multiplier
		void zoom(float zoom);

		// Retrieves the camera offset calculated based on
		// current camera position and screen size
		sf::Vector2f getCameraOffset();

		// Current camera view with rotation and zoom applied
		sf::View getCameraView();

		sf::View getInterfaceView();

		sf::Vector2f getCenter();
		sf::Vector2f getVirtualCenter();

		sf::Vector2f screenToWorld(sf::Vector2f point);
		sf::Vector2f worldToScreen(sf::Vector2f point);

		sf::FloatRect getCameraBounds();

	}
	
}

#endif // SZEN_CAMERA_HPP
