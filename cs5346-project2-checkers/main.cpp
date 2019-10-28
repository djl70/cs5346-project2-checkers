#include "checkers_game_engine.h"
#include "config.h"

int main()
{
	CheckersGameEngine game(config::kFps);
	game.run();

	return 0;
}