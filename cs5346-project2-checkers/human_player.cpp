#include "human_player.h"

#include "jump_command.h"
#include "move_command.h"

HumanPlayer::HumanPlayer(CheckerColor color)
	: Player{ color }
{

}

Command* HumanPlayer::takeTurn(CheckersGameState& gameState)
{
	return nullptr;
}