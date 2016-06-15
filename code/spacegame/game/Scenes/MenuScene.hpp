#ifndef GAME_MENUSCENE_HPP
#define GAME_MENUSCENE_HPP

#include <szen/szen.hpp>

class GameScene;

class MenuScene : public sz::Scene
{
public:
	MenuScene(GameScene*);
	~MenuScene();

	void load();
	void unload();

	void events(const sf::Event& event);
	void update();

	void render();

	bool drawText(sf::Vector2f pos, const std::string& str, float opacity = 1.f, float size = 1.f, float centering = 1.f, bool hoverable = true);

	void drawMainMenu(float phase);
	void drawCredits(float phase);
	void drawSettings(float phase);

	void loadSettings();
	void saveSettings();
	void cancelSettings();

private:
	
	sf::Vector2f m_relativeScale;

	enum MenuState
	{
		Main,
		Settings,
		Credits
	};

	MenuState		m_menuState, m_lastMenuState;

	sf::Sprite		m_help;

	void changeMenu(MenuState state);
	bool lazyIsMenuShowing(MenuState state);

	sf::FloatRect		m_mouseRect;

	struct SettingsData
	{
		sf::Vector2u	fullresolution;
		sf::Vector2u	windowresolution;

		int32			screenmode;
		float			musicVol;
		float			soundVol;
	};

	SettingsData		m_settingsData, m_savedSettingsData;

	bool				m_isPaused;

	sf::RenderWindow*	m_window;

	float			m_menuPhase;

	std::vector<bool>	m_lastHovers, m_currentHovers;

	sf::Clock		m_timer;

	GameScene*		m_gameScene;

	sf::Font*		m_font;

	sf::Sprite		m_logoSprite;
	sf::Sprite		m_borderGlow;

	sf::Sprite		m_credits;

	sf::Sprite		m_volumeScale;

	sf::Shader*		m_glowShader;
	sf::Shader*		m_volumeShader;

	sf::Sprite		m_bluefox;

	bool			m_clearInput;

};

#endif // GAME_MENUSCENE_HPP
