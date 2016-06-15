#include <game/GameGlobals.hpp>
#include <game/Entities/Misc/GameMessage.hpp>

namespace
{
	GameGlobalsPtr	instance;
}

////////////////////////////////////////////////////
GameGlobals::GameGlobals() :
	player				(NULL),
	impact				(NULL),
	impactbits			(NULL),
	m_isBossSpawned		(false)
{
	
}

////////////////////////////////////////////////////
GameGlobalsPtr GameGlobals::get()
{
	if(!instance)
	{
		instance.reset(new GameGlobals);
	}

	return instance;
}

////////////////////////////////////////////////////
GameGlobals::~GameGlobals()
{
	
}

////////////////////////////////////////////////////
void GameGlobals::addMessage(const std::string& str)
{
	gameMessage->addMessage(str);
}

////////////////////////////////////////////////////
void GameGlobals::setBossSpawned(const bool v)
{
	m_isBossSpawned = v;
}

////////////////////////////////////////////////////
bool GameGlobals::isBossSpawned()
{
	return m_isBossSpawned;
}