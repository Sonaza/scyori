#ifndef SZEN_SCENEMANAGER_HPP
#define SZEN_SCENEMANAGER_HPP

#include <memory>
#include <cassert>

#include <szen/System/Scene.hpp>
#include <SFML/System.hpp>

namespace sz
{
	
	typedef std::unique_ptr<Scene> ScenePtr;

	class Core;

	class SceneManager
	{
		friend class Core;

	public:

		static void setScene(Scene*);
		
		static void setOverlayScene(Scene*);
		static void toggleOverlay(bool enabled);

		static bool isOverlayEnabled();

		static void handleEvents(const sf::Event &event);

		static void update();
		static void render();

	protected:

		static void init(Core*);

	private:

		// Use as a static class only, no instances!
		SceneManager(){}

	};
	
}

#endif // SZEN_SCENEMANAGER_HPP
