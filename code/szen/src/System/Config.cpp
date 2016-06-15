#include <szen/System/Config.hpp>

#include <szen/System/ErrorStream.hpp>

#include <json/json.h>
#include <boost/algorithm/string.hpp>
//#include <boost/filesystem.hpp>

/////////////////////////////////////////////////
// DEFAULTS

#define DEFAULT_FULLSCREEN				0
#define DEFAULT_RESOLUTION_FULLSCREEN	Window::getOptimalResolution(true)
#define DEFAULT_RESOLUTION_WINDOWED		Window::getOptimalResolution(false)

#define DEFAULT_VOLUME_SOUND			90.f
#define DEFAULT_VOLUME_MUSIC			50.f

/////////////////////////////////////////////////

namespace
{

	const std::string m_settingsFile = "settings.cfg";

	std::unique_ptr<json::Value>	m_settings;

	bool exists(const std::string& path)
	{
		std::ifstream file(path);
		return file.good();
	}
}

#define _s (*m_settings)

using namespace sz;

////////////////////////////////////////////////////
void setResolutionSettings(int set = -1)
{
	if(set == -1 || set == 0)
	{
		sf::VideoMode window = DEFAULT_RESOLUTION_WINDOWED;
		_s["resolution"]["window"][0U]	= window.width;
		_s["resolution"]["window"][1U]	= window.height;
	}
	
	if(set == -1 || set == 1)
	{
		sf::VideoMode full = DEFAULT_RESOLUTION_FULLSCREEN;
		_s["resolution"]["full"][0U]	= full.width;
		_s["resolution"]["full"][1U]	= full.height;
	}
}

////////////////////////////////////////////////////
void resetSettings()
{
	_s.clear();

	_s["fullscreen"]		= DEFAULT_FULLSCREEN;

	setResolutionSettings();

	_s["volume"]["sound"]	= DEFAULT_VOLUME_SOUND;
	_s["volume"]["music"]	= DEFAULT_VOLUME_MUSIC;

	Config::saveSettings();
}

////////////////////////////////////////////////////
bool hasKey(const std::string& key)
{
	return m_settings->isMember(key);
}

////////////////////////////////////////////////////
void checkKeys()
{
	//////////////////////////////////////////////
	// Check screen keys

	if(!hasKey("fullscreen")) _s["fullscreen"] = DEFAULT_FULLSCREEN;

	if(!hasKey("resolution"))
	{
		setResolutionSettings();
	}
	else
	{
		if(!_s["resolution"].isMember("window")) setResolutionSettings(0);
		if(!_s["resolution"].isMember("full")) setResolutionSettings(1);
	}

	//////////////////////////////////////////////
	// Check audio keys

	if(!hasKey("volume"))
	{
		_s["volume"]["sound"]	= DEFAULT_VOLUME_SOUND;
		_s["volume"]["music"]	= DEFAULT_VOLUME_MUSIC;
	}
	else
	{
		if(!_s["volume"].isMember("music"))
		{
			_s["volume"]["music"]	= DEFAULT_VOLUME_MUSIC;
		}
		else
		{
			_s["volume"]["music"] = std::max(0.f, std::min(100.f, (float)_s["volume"]["music"].asDouble()));
		}

		if(!_s["volume"].isMember("sound"))
		{
			_s["volume"]["sound"]	= DEFAULT_VOLUME_SOUND;
		}
		else
		{
			_s["volume"]["sound"] = std::max(0.f, std::min(100.f, (float)_s["volume"]["sound"].asDouble()));
		}
	}
}

////////////////////////////////////////////////////
void openSettings()
{
	std::ifstream file("settings.cfg");

	json::Reader reader;
	if(!reader.parse(file, *m_settings))
	{
		szerr << "Parse error in settings file, resetting to defaults." << ErrorStream::error;
		resetSettings();
	}
	else
	{
		checkKeys();
	}
}

////////////////////////////////////////////////////
void Config::saveSettings()
{
	// Attempt to open settings file
	std::ofstream file(m_settingsFile, std::ios_base::trunc);
	if(!file.is_open())
	{
		szerr << "Unable to open settings file '" << m_settingsFile << "' for writing!" << ErrorStream::error;
		return;
	}

	// Write config to file
	json::StyledWriter writer;
	file << writer.write(*m_settings);
	file.close();
}

////////////////////////////////////////////////////
void Config::initSettingsFile()
{
	m_settings.reset(new json::Value);

	// If settings file already exists don't do anything
	if(exists("settings.cfg"))
	{
		openSettings();
	}
	else
	{
		resetSettings();
	}
}

////////////////////////////////////////////////////
void Config::closeSettings()
{
	saveSettings();
	m_settings.release();
}

////////////////////////////////////////////////////
void Config::setScreenMode(uint32 value)
{
	if(value >= 3) value = 0;

	_s["fullscreen"] = value;
	saveSettings();
}

////////////////////////////////////////////////////
bool Config::isFullscreen()
{
	return getScreenMode() >= 1;
}

////////////////////////////////////////////////////
uint32 Config::getScreenMode()
{
	return _s["fullscreen"].asUInt();
}

////////////////////////////////////////////////////
uint32 Config::getSystemWindowMode()
{
	switch(getScreenMode())
	{
		case 0: return sf::Style::Close;			// Windowed
		case 1: return sf::Style::Fullscreen;		// Normal fullscreen
		case 2: return sf::Style::None;				// Windowed fullscreen
	}

	return sf::Style::Close;
}

////////////////////////////////////////////////////
sf::VideoMode Config::getVideoMode()
{
	return getVideoMode(getScreenMode());
}

////////////////////////////////////////////////////
bool checkAspect(sf::VideoMode mode)
{
	sf::VideoMode optimal = sz::Window::getOptimalResolution(Config::isFullscreen());
	float optimalAspect = optimal.width / static_cast<float>(optimal.height);

	float userAspect = mode.width / static_cast<float>(mode.height);

	if(fabs(optimalAspect-userAspect) <= 0.001f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////
sf::VideoMode Config::getVideoMode(const uint32 mode)
{
	sf::VideoMode result;

	switch(mode)
	{
		////////////////////////////////////////////
		// Windowed
		case 0:
		{
			result = sf::VideoMode(
				_s["resolution"]["window"][0U].asUInt(),
				_s["resolution"]["window"][1U].asUInt()
			);

			break;
		}

		////////////////////////////////////////////
		// Fullscreen
		case 1:
		{
			result = sf::VideoMode(
				_s["resolution"]["full"][0U].asUInt(),
				_s["resolution"]["full"][1U].asUInt()
			);

			break;
		}

		////////////////////////////////////////////
		// Windowed fullscreen
		case 2:
		{
			result = sz::Window::getOptimalResolution(true);
			break;
		}
	}


	return result;
}

////////////////////////////////////////////////////
void Config::setVideoMode(const bool isFull, sf::VideoMode mode)
{
	if(isFull)
	{
		_s["resolution"]["full"][0U] = mode.width;
		_s["resolution"]["full"][1U] = mode.height;
	}
	else
	{
		_s["resolution"]["window"][0U] = mode.width;
		_s["resolution"]["window"][1U] = mode.height;
	}
}

////////////////////////////////////////////////////
float Config::getSoundVolume()
{
	return static_cast<float>(_s["volume"]["sound"].asDouble());
}

////////////////////////////////////////////////////
void Config::setSoundVolume(float value)
{
	_s["volume"]["sound"] = std::max(0.f, std::min(100.f, value));
}

////////////////////////////////////////////////////
float Config::getMusicVolume()
{
	return static_cast<float>(_s["volume"]["music"].asDouble());
}

////////////////////////////////////////////////////
void Config::setMusicVolume(float value)
{
	_s["volume"]["music"] = std::max(0.f, std::min(100.f, value));
}
