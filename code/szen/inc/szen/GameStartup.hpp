#ifndef SZEN_GAMESTARTUP_HPP
#define SZEN_GAMESTARTUP_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

namespace sz
{
	
	class Scene;
	class Core;

	typedef std::unique_ptr<Core> CorePtr;

	class GameStartup
	{
		friend class Core;

	public:
		GameStartup();
		~GameStartup();

		void run();

		virtual void initOptions() =0;
		virtual void startup() =0;

		void createScene(Scene*);

		template <class T>
		void createScene();

	protected:

		CorePtr		m_core;

		struct Settings
		{
			Settings();

			sf::VideoMode	videomode;
			sf::Uint32		virtualWidth;

			std::string		windowTitle;
			sf::Uint32		windowMode;
			sf::Uint32		antialiasing;

			sf::Vector2f	gravity;

		} Options;

	};

	////////////////////////////////////////////////////
	template <class T>
	void GameStartup::createScene()
	{
		createScene(new(std::nothrow) T);
	}
	
}

#endif // SZEN_GAMESTARTUP_HPP
