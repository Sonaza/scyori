#include <szen/System/Window.hpp>
#include <szen/Game/Camera.hpp>

#include <sstream>

#ifdef SFML_SYSTEM_WINDOWS
	#include <Windows.h>
#endif

using namespace sz;

namespace
{
	sf::RenderWindow*	m_window = NULL;
	
	float		m_aspectRatio;
	sf::Uint32	m_virtualWidth	= 1920;

	sf::View	m_view;

	std::string		m_title;
	sf::Uint32		m_antialiasing;
}

////////////////////////////////////////////////////
void Window::open(sf::VideoMode videomode, const std::string &title,
				  const sf::Uint32 style, const sf::Uint32 antialias,
				  const sf::Uint32 virtualWidth)
{
	assert(!m_window && "Window is already open");

	sf::ContextSettings settings;
	settings.antialiasingLevel = antialias;

	// Create new window instance
	m_window = new(std::nothrow) sf::RenderWindow(videomode, title, style, settings);
	assert(m_window && "Allocation failed");

	m_title = title;
	m_antialiasing = antialias;

	m_aspectRatio = videomode.width / static_cast<float>(videomode.height);
	
	// Set virtual width
	m_virtualWidth = (virtualWidth == 0 ? videomode.width : virtualWidth);

	m_window->setVerticalSyncEnabled(true);

	m_window->clear(sf::Color::Black);
	m_window->display();

	Camera::updateScreenSize();
}

////////////////////////////////////////////////////
void Window::changeMode(sf::VideoMode videomode, const sf::Uint32 style)
{
	if(!m_window) return;

	// Delete old window and create new instead
	delete m_window;

	sf::ContextSettings settings;
	settings.antialiasingLevel = m_antialiasing;

	m_window = new sf::RenderWindow(videomode, m_title, style, settings);

	m_aspectRatio = videomode.width / static_cast<float>(videomode.height);

	m_window->setVerticalSyncEnabled(true);

	m_window->clear(sf::Color::Black);
	m_window->display();

	Camera::updateScreenSize();
}

////////////////////////////////////////////////////
void Window::close()
{
	if(m_window)
	{
		m_window->close();

		delete m_window;
		m_window = NULL;
	}
}

////////////////////////////////////////////////////
sf::Vector2u Window::getSize()
{
	return m_window->getSize();
}

////////////////////////////////////////////////////
void Window::setVirtualWidth(const sf::Uint32 width)
{
	m_virtualWidth = width;

	Camera::updateScreenSize();
}

////////////////////////////////////////////////////
sf::Vector2u Window::getVirtualSize()
{
	return sf::Vector2u(
		m_virtualWidth,
		static_cast<sf::Uint32>(ceil(m_virtualWidth * (1.f / m_aspectRatio))));
}

////////////////////////////////////////////////////
sf::RenderWindow* Window::getRenderWindow()
{
	return m_window;
}

////////////////////////////////////////////////////
bool Window::isActive()
{
	// Windows implementation
	return m_window->getSystemHandle() == GetActiveWindow();
}

////////////////////////////////////////////////////////////
sf::VideoMode Window::getOptimalResolution(const bool fullscreen)
{
	sf::VideoMode native = sf::VideoMode::getDesktopMode();
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

	sf::VideoMode videomode = native;

	if(fullscreen && native == modes[0]) return videomode;

	float nativeRatio = native.width / static_cast<float>(native.height);
	float ratio;

	for(std::vector<sf::VideoMode>::iterator it = modes.begin(); it != modes.end(); ++it)
	{
		sf::VideoMode mode = *it;

		if(mode.bitsPerPixel != native.bitsPerPixel) continue;
		if(mode.width >= native.width) continue;

		ratio = mode.width / static_cast<float>(mode.height);

		if(fabs(nativeRatio - ratio) <= 0.001f)
		{
			videomode = mode;
			break;
		}
	}

	return videomode;
}

#include <iostream>

////////////////////////////////////////////////////////////
int calculateGCD(int a, int b) {
	return (b == 0) ? a : calculateGCD (b, a%b);
}

////////////////////////////////////////////////////////////
std::vector<sf::VideoMode> Window::getSupportedResolutions(const bool fullscreen)
{
	sf::VideoMode native = sf::VideoMode::getDesktopMode();
	float nativeAspect = native.width / (float)native.height;

	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	std::vector<sf::VideoMode> result;

	for(int i=0; i < modes.size(); ++i)
	{
		sf::VideoMode mode = modes[i];
		bool cond = true;

		float ratio = mode.width / (float)mode.height;

		cond &= mode.bitsPerPixel == modes[0].bitsPerPixel;
		cond &= mode.width >= 1024;
		cond &= mode.width <= native.width && mode.height <= native.height;
		//cond &= nativeAspect == ratio;

		if(cond)
		{
			result.push_back(mode);
		}
	}

	std::sort(result.begin(), result.end(),
		[](sf::VideoMode& a, sf::VideoMode& b) -> bool
		{
			/*float ra = a.width / (float)a.height;
			float rb = b.width / (float)b.height;
			*/
			return (a.width * a.height) < (b.width * b.height);
		}
	);

	std::cout << "============================================================\n";

	for(int i=0; i < result.size(); ++i)
	{
		sf::VideoMode mode = result[i];

		std::stringstream aspect;

		int gcd = calculateGCD(mode.width, mode.height);
		sf::Vector2i ratio(mode.width, mode.height);
		ratio /= gcd;

		if(ratio.x == 8 && ratio.y == 5) ratio *= 2;

		aspect << ratio.x << ":" << ratio.y;

		std::cout << mode.width << " x " << mode.height << " (" << aspect.str() << ")\n";
	}

	return result;
}