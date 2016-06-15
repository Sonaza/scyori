#include <game/Entities/Misc/GameMessage.hpp>
#include <game/GameGlobals.hpp>
#include <game/Entities/Actors/PlayerEntity.hpp>

////////////////////////////////////////////////////
GameMessage::GameMessage()
{
	loadFromPrefab("data/game_message.szt");

	m_messageShowing = false;
	m_messagePhase = 0.f;
}

////////////////////////////////////////////////////
GameMessage::~GameMessage()
{
	
}

////////////////////////////////////////////////////
void GameMessage::update()
{
	bool playerDead = GameGlobals::get()->player->isDead();
	if(playerDead && (m_messageShowing || !m_messageQueue.empty()))
	{
		m_messageQueue.clear();
		getTextRenderer->setText("");
		m_messageShowing = false;
	}

	bool isExpired = m_messageTimer.getElapsedTime() >= sf::seconds(1.25f);

	if(isExpired)
	{
		if(!m_messageQueue.empty())
		{
			m_messageTimer.restart();

			m_currentMessage = m_messageQueue.front();
			m_messageQueue.pop_front();

			getTextRenderer->setText(m_currentMessage);

			m_messageShowing = true;

			m_messagePhase = 2.f;
		}
		else if(m_messageShowing)
		{
			//getTextRenderer->setText("");
			m_messageShowing = false;
		}
	}

	float scale = 1.f;
	float opacity = 1.f;

	if(m_messageShowing)
	{
		m_messagePhase += (1.f - m_messagePhase) / 8.f;

		float x = m_messagePhase-1.f;
		opacity = -x * x + 1.f;

		scale = m_messagePhase * 0.5f + 0.5f;
	}
	else
	{
		m_messagePhase += (0.f - m_messagePhase) / 8.f;

		float x = (m_messagePhase - 1.f) * 1.34f;
		opacity = -x * x + 1.f;

		scale = m_messagePhase * 0.25f + 0.75f;
	}

	getTextRenderer->setOpacity(opacity);
	getTransform->setScale(scale, scale);
}

////////////////////////////////////////////////////
void GameMessage::addMessage(const std::string& msg)
{
	m_messageQueue.push_back(msg);
}