//#define VLD

#if defined(_DEBUG) && defined(VLD)
	#include <vld.h>
	//#pragma comment(linker, "vld.lib")
#endif

#include <game/Game.hpp>

int main()
{
	Game::launch();
}
