#include "Game.h"

int main(int argc, char ** arvg)
{
	Game game;
	bool success = game.Initialize();

	if (success)
	{
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}