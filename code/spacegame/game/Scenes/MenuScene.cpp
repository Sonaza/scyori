#include <game/Scenes/MenuScene.hpp>

#include <game/Scenes/GameScene.hpp>

////////////////////////////////////////////////////
MenuScene::MenuScene(GameScene* gs) :
	m_gameScene		(gs),
	m_menuState		(Main),
	m_lastHovers	(Main),
	m_menuPhase		(0.f),
	m_isPaused		(false)
{
	
}

////////////////////////////////////////////////////
MenuScene::~MenuScene()
{
	
}

////////////////////////////////////////////////////
void MenuScene::load()
{
	m_relativeScale.x = Screen.width / 1920.f;
	m_relativeScale.y = Screen.height / 1080.f;

	auto logo = sz::Asset::loadTexture("mainlogo", "data/mainlogo.png", false)->getAsset();
	m_logoSprite.setTexture(*logo);
	m_logoSprite.setOrigin(750.f, 250.f);
	m_logoSprite.setScale(m_relativeScale.y, m_relativeScale.y);

	auto glow = sz::Asset::loadTexture("borderglow", "data/mainlogo_border.png", false)->getAsset();
	m_borderGlow.setTexture(*glow);
	m_borderGlow.setOrigin(750.f, 250.f);
	m_borderGlow.setScale(m_relativeScale.y, m_relativeScale.y);

	m_glowShader = sz::Asset::loadShader("borderglow", "data/effects/borderglow.frag", false)->getAsset();
	m_volumeShader = sz::Asset::loadShader("volumeshader", "data/effects/volumescale.frag", false)->getAsset();

	auto volumescale = sz::Asset::loadTexture("volumescale", "data/volumescale.png", false)->getAsset();
	m_volumeScale.setTexture(*volumescale);
	m_volumeScale.setOrigin(300.f, 10.f);
	m_volumeScale.setScale(m_relativeScale.y, m_relativeScale.y);

	auto help = sz::Asset::loadTexture("help", "data/help.png", false)->getAsset();
	m_help.setTexture(*help);
	m_help.setOrigin(310.f, 0.f);
	m_help.setScale(m_relativeScale.y, m_relativeScale.y);

	auto bluefox = sz::Asset::loadTexture("bluefoxlogo", "data/bluefox.png", false)->getAsset();
	m_bluefox.setTexture(*bluefox);
	m_bluefox.setOrigin(-15.f, 100.f);
	m_bluefox.setScale(m_relativeScale.y * 0.75f, m_relativeScale.y * 0.75f);

	auto mask = sz::Asset::loadTexture("glowmask", "data/sweepmask.png", false)->getAsset();
	m_glowShader->setParameter("u_mask", *mask);

	m_font = sz::Asset::getFont("powerchord")->getAsset();
	
	sz::Camera::setPosition(sf::Vector2f(
		cos(0.f) * 2000.f - 1000.f,
		sin(0.f) * 1000.f + 2000.f
	));

	auto credits = sz::Asset::loadTexture("credits", "data/credits.png", false)->getAsset();
	m_credits.setTexture(*credits);
	m_credits.setOrigin(750.f, 500.f);
	m_credits.setScale(m_relativeScale.x, m_relativeScale.x);

	loadSettings();
}

////////////////////////////////////////////////////
void MenuScene::unload()
{

}

////////////////////////////////////////////////////
void MenuScene::events(const sf::Event& event)
{

}

////////////////////////////////////////////////////
void MenuScene::update()
{
	m_relativeScale.x = Screen.width / 1920.f;
	m_relativeScale.y = Screen.height / 1080.f;

	m_logoSprite.setScale(m_relativeScale.y, m_relativeScale.y);
	m_borderGlow.setScale(m_relativeScale.y, m_relativeScale.y);
	m_volumeScale.setScale(m_relativeScale.y, m_relativeScale.y);
	m_credits.setScale(m_relativeScale.x, m_relativeScale.x);
	m_help.setScale(m_relativeScale.y, m_relativeScale.y);

	if(!m_clearInput && !sz::Input::isMouseDown(sf::Mouse::Left))
	{
		m_clearInput = true;
	}

	m_isPaused = sz::Scene::isPaused();

	auto mouse = static_cast<sf::Vector2f>(sz::Input::getMousePos());
	m_mouseRect = sf::FloatRect(mouse.x, mouse.y, 1.f, 1.f);

	float time = m_timer.getElapsedTime().asSeconds();

	if(!m_gameScene->isGameStarted())
	{
		sf::Vector2f target(
			cos(time * 0.05f) * 2000.f - 1000.f,
			sin(time * 0.5f) * 1000.f + 2000.f
		);

		sf::Vector2f current = sz::Camera::getPosition();

		sf::Vector2f dir = target - current;
		float len = sz::lengthsqrt(dir);
		if(len > 0.f)
		{
			dir = sz::normalize(dir) * len / 60.f;
			sz::Camera::setPosition(current + dir);
			sz::Camera::setZoom(2.5f);
		}
	}

	m_glowShader->setParameter("u_time", time);

	if(m_menuState == Main)
	{
		m_menuPhase += (-0.05f - m_menuPhase) / 5.f;
	}
	else
	{
		m_menuPhase += (1.05f - m_menuPhase) / 5.f;
	}

	m_menuPhase = std::max(0.f, std::min(1.f, m_menuPhase));

	switch(m_menuState)
	{
		/////////////////////////////////////////////
		case Main:
			if(sz::Input::isKeyHit(sf::Keyboard::Escape))
			{
				if(m_isPaused && m_gameScene->isGameStarted())
				{
					sz::Scene::togglePause(false);
					sz::SceneManager::toggleOverlay(false);
				}
				else if(!m_gameScene->isGameStarted())
				{
					sz::Core::quit();
				}
			}
		break;

		/////////////////////////////////////////////
		case Settings:
			if(sz::Input::isKeyHit(sf::Keyboard::Escape))
			{
				cancelSettings();

				changeMenu(Main);
			}
		break;

		/////////////////////////////////////////////
		case Credits:
			if(sz::Input::isMouseHit(sf::Mouse::Left) || sz::Input::isKeyHit(sf::Keyboard::Escape))
			{
				changeMenu(Main);
			}
		break;
	}
}

////////////////////////////////////////////////////
void MenuScene::render()
{
	m_window = sz::Window::getRenderWindow();

	if(m_isPaused)
	{
		sf::Vector2f size(Screen.width + 20.f, Screen.height + 20.f);
		sf::RectangleShape backbar(size);
		backbar.setFillColor(sf::Color(0, 0, 0, 80));
		backbar.setOrigin(size * 0.5f);
		backbar.setPosition(Screen.width / 2.f, Screen.height / 2.f);

		m_window->draw(backbar);
	}

	if(lazyIsMenuShowing(Settings)) drawSettings(m_menuPhase);

	drawMainMenu(m_menuPhase);

	if(lazyIsMenuShowing(Credits)) drawCredits(m_menuPhase);
}

////////////////////////////////////////////////////
void MenuScene::changeMenu(MenuState state)
{
	m_lastMenuState = m_menuState;
	m_menuState = state;

	sz::Input::clearInputs();
	m_clearInput = false;
}

////////////////////////////////////////////////////
bool MenuScene::lazyIsMenuShowing(MenuState state)
{
	return m_menuState == state || m_lastMenuState == state;
}

#include <Windows.h>
#include <ShellAPI.h>

////////////////////////////////////////////////////
void MenuScene::drawMainMenu(float phase)
{
	if(m_currentHovers.empty())
	{
		m_currentHovers.resize(10, false);
	}

	bool menuActive = m_menuState == Main;

	float phaseOffset = lazyIsMenuShowing(Settings) ? 0.05f : 0.08f;

	sf::Vector2f logoPosition(
		Screen.width * 0.5f,
		Screen.height * 0.28f - phase * Screen.height * phaseOffset
	);

	m_logoSprite.setPosition(logoPosition);
	m_window->draw(m_logoSprite);

	{
		m_borderGlow.setPosition(logoPosition);

		sf::RenderStates states;
		states.blendMode = sf::BlendAdd;
		states.shader = m_glowShader;

		m_window->draw(m_borderGlow, states);
	}

	/////////////////////////

	sf::Color c(255, 255, 255, 255-static_cast<sf::Uint8>(std::min<float>(255, phase * 255.f)));

	auto helppos = logoPosition + sf::Vector2f(m_relativeScale.x * -230.f, m_relativeScale.y * 150.f);
	m_help.setColor(c);
	m_help.setPosition(helppos);
	m_window->draw(m_help);

	///////////////////////////////////

	{
		m_bluefox.setColor(c);
		auto logopos = sf::Vector2f(0.f, Screen.height);
		m_bluefox.setPosition(logopos);

		bool hover = m_mouseRect.intersects(m_bluefox.getGlobalBounds());
		if(m_menuState == Main && hover)
		{
			m_bluefox.setPosition(logopos + sf::Vector2f(0.f, -5.f));
			if(sz::Input::isMouseHit(sf::Mouse::Left))
			{
				ShellExecute(NULL, "open", "http://bluefoxgames.org/scyori/?rel=game",
					NULL, NULL, SW_SHOWNORMAL);
			}
		}

		m_window->draw(m_bluefox);
	}

	///////////////////////////////////

	float xpos = Screen.width * 0.5f + (385.f + 50.f) * m_relativeScale.y;

	float spacing = Screen.height * 0.08f;

	std::vector<std::string> items;

	if(!m_isPaused)
	{
		items.push_back("New game");
		items.push_back("Settings");
		items.push_back("Credits");
	}
	else
	{
		items.push_back("Resume");
		items.push_back("Settings");
		items.push_back("Retire");
	}

	items.push_back("Quit");

	int32 hovered = -1;

	if(menuActive) m_lastHovers = m_currentHovers;

	for(auto i = 0U; i < items.size(); ++i)
	{
		bool hover = drawText(
			sf::Vector2f(xpos, logoPosition.y + spacing * 2.f + i * spacing), items[i], 1.f-phase);

		if(!menuActive) continue;

		if(hover) hovered = i;

		m_currentHovers[i] = hover;

		if(sz::Window::isActive() && hover && m_lastHovers[i] != m_currentHovers[i])
		{
			sz::Asset::getSound("menu_select")->play(50.f);
		}
	}

	if(m_menuState == Main && sz::Window::isActive() && sz::Input::isMouseHit(sf::Mouse::Left))
	{
		bool playSound = true;

		if(!m_isPaused)
		{
			switch(hovered)
			{
				////////////////////////////////////////////////////
				case 0:
					sz::SceneManager::toggleOverlay(false);
					m_gameScene->startGame();

					sz::Input::clearInputs();
				break;

				////////////////////////////////////////////////////
				case 1:
					changeMenu(Settings);
				break;

				////////////////////////////////////////////////////
				case 2:
					changeMenu(Credits);
				break;

				////////////////////////////////////////////////////
				case 3:
					sz::Core::quit();
				break;

				default:
					playSound = false;
				break;
			}
		}
		else
		{
			switch(hovered)
			{
				////////////////////////////////////////////////////
				case 0:
					sz::SceneManager::toggleOverlay(false);
					sz::Scene::togglePause(false);

					sz::Input::clearInputs();
				break;

				////////////////////////////////////////////////////
				case 1:
					changeMenu(Settings);
				break;

				////////////////////////////////////////////////////
				case 2:
					m_gameScene->stopGame();
				break;

				////////////////////////////////////////////////////
				case 3:
					sz::Core::quit();
				break;

				default:
					playSound = false;
				break;
			}
		}

		if(playSound)
		{
			sz::Asset::getSound("menu_confirm")->play(80.f);
		}
	}
}

////////////////////////////////////////////////////
void MenuScene::drawCredits(float phase)
{
	sf::Color c(255, 255, 255, static_cast<sf::Uint8>(std::min<float>(255, phase * 255.f)));
	m_credits.setColor(c);

	float h = Screen.height * 0.08f;
	m_credits.setPosition(Screen.width * 0.5f, Screen.height * 0.76f + (1.f-phase) * h);
	m_window->draw(m_credits);
}

////////////////////////////////////////////////////
void MenuScene::drawSettings(float phase)
{
	sf::Color c(255, 255, 255, static_cast<sf::Uint8>(std::min<float>(255, phase * 255.f)));

	sf::Vector2f logoPosition(
		Screen.width * 0.5f,
		Screen.height * 0.28f - phase * Screen.height * 0.05f
	);

	bool menuActive = m_menuState == Settings;

	float xlpos = Screen.width * 0.5f - 500.f * m_relativeScale.y;
	float xrpos = Screen.width * 0.5f + 500.f * m_relativeScale.y;

	float spacing = Screen.height * 0.08f;

	std::vector<std::string> labels;

	labels.push_back("Resolution");
	labels.push_back("Screen Mode");
	labels.push_back("Music Volume");
	labels.push_back("Sound Volume");

	int32 hovered = -1;
	
	if(menuActive) m_lastHovers = m_currentHovers;
	//std::fill(m_currentHovers.begin(), m_currentHovers.end(), false);

	for(auto i = 0U; i < labels.size(); ++i)
	{
		bool hover = false;

		drawText(
			sf::Vector2f(xlpos, logoPosition.y + spacing * 2.f + i * spacing * 0.9f),
			labels[i], phase, 0.84f, 0.f, false);

		switch(i)
		{
			/////////////////////////////////////
			case 0:
			{
				std::stringstream resolutionText;

				switch(m_settingsData.screenmode)
				{
					//////////////////////////////////////
					case 0:
						resolutionText << m_settingsData.windowresolution.x << " x " << m_settingsData.windowresolution.y;
					break;

					//////////////////////////////////////
					case 1:
						resolutionText << m_settingsData.fullresolution.x << " x " << m_settingsData.fullresolution.y;
					break;

					//////////////////////////////////////
					case 2:
					{
						auto native = sf::VideoMode::getDesktopMode();
						resolutionText << native.width << " x " << native.height;
					break;
					}
				}

				hover = drawText(
					sf::Vector2f(xrpos, logoPosition.y + spacing * 2.f + i * spacing * 0.9f),
					resolutionText.str(), phase, 0.84f);
			break;
			}

			/////////////////////////////////////
			case 1:
			{
				std::string modeText;
				uint32 mode = m_settingsData.screenmode;

				if(mode == 0)		modeText	= "Windowed";
				else if(mode == 1)	modeText	= "Fullscreen";
				else if(mode == 2)	modeText	= "Full windowed";

				hover = drawText(
					sf::Vector2f(xrpos, logoPosition.y + spacing * 2.f + i * spacing * 0.9f),
					modeText, phase, 0.84f);
			break;
			}

			/////////////////////////////////////
			case 2:
			case 3:
			{
				m_volumeScale.setColor(c);
				m_volumeScale.setPosition(xrpos, logoPosition.y + spacing * 2.f + i * spacing * 0.9f);

				auto bounds = m_volumeScale.getGlobalBounds();
				hover = m_mouseRect.intersects(bounds);

				float* vol = NULL;
				if(i == 2)		vol = &m_settingsData.musicVol;
				else if(i == 3)	vol = &m_settingsData.soundVol;
				
				m_volumeShader->setParameter("u_progress", *vol / 100.f);

				if(hover)	m_volumeShader->setParameter("u_hover", 1.f);
				else		m_volumeShader->setParameter("u_hover", 0.f);

				m_window->draw(m_volumeScale, m_volumeShader);

				if(menuActive && hover && m_clearInput && sz::Input::isMouseDown(sf::Mouse::Left))
				{
					float offset = (m_mouseRect.left - bounds.left) / bounds.width;
					offset = std::max(0.f, std::min(1.f, offset)) * 100.f;

					offset = (float)((int)(offset / 2.5f + 0.5f) * 2.5f);

					*vol = offset;

					if(i == 2)
					{
						sz::Config::setMusicVolume(*vol);
					}
					else
					{
						sz::Config::setSoundVolume(*vol);
					}
				}
			break;
			}
		} // switch

		if(!menuActive) continue;

		if(hover) hovered = i;

		m_currentHovers[i] = hover;

		if(sz::Window::isActive() && hover && m_lastHovers[i] != m_currentHovers[i])
		{
			sz::Asset::getSound("menu_select")->play(50.f);
		}
	}

	bool cancelHover = drawText(
		sf::Vector2f(Screen.width * 0.5f - 200.f * m_relativeScale.x, logoPosition.y + spacing * 2.f + 4.4f * spacing * 0.9f),
		"Cancel", phase, 0.84f, 0.5f, true) && menuActive;

	m_currentHovers[4] = cancelHover;
	if(sz::Window::isActive() && cancelHover && m_lastHovers[4] != m_currentHovers[4])
	{
		sz::Asset::getSound("menu_select")->play(50.f);
	}

	bool applyHover = drawText(
		sf::Vector2f(Screen.width * 0.5f + 200.f * m_relativeScale.x, logoPosition.y + spacing * 2.f + 4.4f * spacing * 0.9f),
		"Apply", phase, 0.84f, 0.5f, true) && menuActive;

	m_currentHovers[5] = applyHover;
	if(sz::Window::isActive() && applyHover && m_lastHovers[5] != m_currentHovers[5])
	{
		sz::Asset::getSound("menu_select")->play(50.f);
	}

	if(menuActive)
	{
		bool leftClick = sz::Input::isMouseHit(sf::Mouse::Left);
		bool rightClick = sz::Input::isMouseHit(sf::Mouse::Right);

		if(leftClick)
		{
			if(cancelHover)
			{
				cancelSettings();

				sz::Asset::getSound("menu_back")->play(80.f);
				changeMenu(Main);
			}
			else if(applyHover)
			{
				saveSettings();

				sz::Asset::getSound("menu_confirm")->play(80.f);
				changeMenu(Main);
			}
		}

		if(leftClick || rightClick)
		{
			int dir = leftClick ? 1 : rightClick ? -1 : 0;

			switch(hovered)
			{
				//////////////////////////////////////////
				case 0:
				{
					if(m_settingsData.screenmode != 2)
					{
						auto resolutions = sz::Window::getSupportedResolutions(false);

						sf::Vector2u* res;
						if(m_settingsData.screenmode == 0)
						{
							res = &m_settingsData.windowresolution;
						}
						else if(m_settingsData.screenmode == 1)
						{
							res = &m_settingsData.fullresolution;
						}

						int i; 
						for(i=0; i < resolutions.size(); ++i)
						{
							auto vm = resolutions[i];
							if(res->x == vm.width && res->y == vm.height)
							{
								break;
							}
						}

						i += dir;
						if(i == resolutions.size()) i=0;
						if(i < 0) i = resolutions.size()-1;

						*res = sf::Vector2u(resolutions[i].width, resolutions[i].height);
					}

					if(leftClick)	sz::Asset::getSound("menu_confirm")->play(80.f);
					else			sz::Asset::getSound("menu_back")->play(80.f);
				break;
				}

				//////////////////////////////////////////
				case 1:
					m_settingsData.screenmode += dir;
					if(m_settingsData.screenmode < 0) m_settingsData.screenmode = 2;
					if(m_settingsData.screenmode > 2) m_settingsData.screenmode = 0;

					if(leftClick)	sz::Asset::getSound("menu_confirm")->play(80.f);
					else			sz::Asset::getSound("menu_back")->play(80.f);
				break;
			}
		}
	}

	if(sz::Input::isKeyHit(sf::Keyboard::Space))
	{
		sz::Window::getSupportedResolutions(false);
	}
}

////////////////////////////////////////////////////
void MenuScene::loadSettings()
{
	auto vm_full	= sz::Config::getVideoMode(true);
	auto vm_window	= sz::Config::getVideoMode(false);
	m_settingsData.fullresolution	= sf::Vector2u(vm_full.width, vm_full.height);
	m_settingsData.windowresolution	= sf::Vector2u(vm_window.width, vm_window.height);

	m_settingsData.screenmode	= sz::Config::getScreenMode();

	m_settingsData.musicVol		= sz::Config::getMusicVolume();
	m_settingsData.soundVol		= sz::Config::getSoundVolume();

	m_savedSettingsData = m_settingsData;
}

////////////////////////////////////////////////////
void MenuScene::saveSettings()
{
	bool updateWindow = m_settingsData.screenmode != m_savedSettingsData.screenmode;
	if(!updateWindow && m_settingsData.screenmode == 0)
	{
		updateWindow = m_settingsData.windowresolution != m_savedSettingsData.windowresolution;
	}
	else if(!updateWindow && m_settingsData.screenmode == 1)
	{
		updateWindow = m_settingsData.fullresolution != m_savedSettingsData.fullresolution;
	}

	if(updateWindow)
	{
		sz::Config::setScreenMode(m_settingsData.screenmode);

		if(m_settingsData.screenmode == 0)
		{
			sz::Config::setVideoMode(false, sf::VideoMode(
				m_settingsData.windowresolution.x, m_settingsData.windowresolution.y
			));
		}
		else if(m_settingsData.screenmode == 1)
		{
			sz::Config::setVideoMode(true, sf::VideoMode(
				m_settingsData.fullresolution.x, m_settingsData.fullresolution.y
			));
		}

		sz::Window::changeMode(
			sz::Config::getVideoMode(),
			sz::Config::getSystemWindowMode()
		);

		m_window = sz::Window::getRenderWindow();
		m_window->setMouseCursorVisible(false);
	}

	sz::Config::setMusicVolume(m_settingsData.musicVol);
	sz::Config::setSoundVolume(m_settingsData.soundVol);

	sz::Config::saveSettings();

	m_savedSettingsData = m_settingsData;
}

////////////////////////////////////////////////////
void MenuScene::cancelSettings()
{
	m_settingsData = m_savedSettingsData;

	sz::Config::setMusicVolume(m_settingsData.musicVol);
	sz::Config::setSoundVolume(m_settingsData.soundVol);
}

////////////////////////////////////////////////////
bool MenuScene::drawText(sf::Vector2f pos, const std::string& str,
				float opacity, float size, float centering, bool hoverable)
{
	sf::Text text(str, *m_font, (int)(Screen.height * 0.07f * size));

	auto bounds = text.getLocalBounds();
	text.setOrigin(
		(bounds.width - bounds.left) * centering,
		(bounds.height - bounds.top) * 0.5f
	);

	text.setPosition(pos + sf::Vector2f(1.f, 3.f));
	text.setColor(sf::Color(
		(sf::Uint8)(205 * 0.1f),
		(sf::Uint8)(139 * 0.1f),
		(sf::Uint8)(76 * 0.1f),
		(sf::Uint8)(255 * opacity)));

	sz::Window::getRenderWindow()->draw(text);

	text.setPosition(pos);

	auto gbounds = text.getGlobalBounds();

	bool isHovering = m_mouseRect.intersects(gbounds);
	if(hoverable)
	{
		if(!isHovering)
		{
			text.setColor(sf::Color(245, 236, 228, (sf::Uint8)(255 * opacity)));
		}
		else
		{
			text.setColor(sf::Color(228, 167, 113, (sf::Uint8)(255 * opacity)));
		}
	}
	else
	{
		text.setColor(sf::Color(255, 255, 255, (sf::Uint8)(255 * opacity)));
	}

	sz::Window::getRenderWindow()->draw(text);

	return isHovering;
}