#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include <szen/GameStartup.hpp>
#include <memory>

class Game : public sz::GameStartup
{
public:
	Game(void);
	~Game(void);

	static void launch();
	
	virtual void initOptions();
	virtual void startup();

};

#endif
