#ifndef SZEN_CONFIG_HPP
#define SZEN_CONFIG_HPP

#include <fstream>
#include <szen/System/Window.hpp>

#include <memory>

#include <szen/System/Types.hpp>

namespace sz
{
	
	namespace Config
	{

		void initSettingsFile();
		void closeSettings();

		void saveSettings();

		///////////////////////////////////

		uint32 getScreenMode();
		void setScreenMode(uint32);

		bool isFullscreen();

		uint32 getSystemWindowMode();

		sf::VideoMode getVideoMode();
		sf::VideoMode getVideoMode(const uint32 fullscreen);

		void setVideoMode(const bool isFull, sf::VideoMode mode);

		float getSoundVolume();
		void setSoundVolume(float value);

		float getMusicVolume();
		void setMusicVolume(float value);

	};
	
}

#endif // SZEN_CONFIG_HPP
