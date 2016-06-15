#ifndef GAME_GAMEMESSAGE_HPP
#define GAME_GAMEMESSAGE_HPP

#include <szen/szen.hpp>

#include <deque>
#include <vector>

class GameMessage : public sz::Entity
{
public:
	GameMessage();
	~GameMessage();

	void update();

	void addMessage(const std::string& msg);

private:

	bool	m_messageShowing;

	std::string		m_currentMessage;

	float				m_messagePhase;
	sz::PausableClock	m_messageTimer;

	std::deque<std::string>	m_messageQueue;

};

#endif // GAME_GAMEMESSAGE_HPP
