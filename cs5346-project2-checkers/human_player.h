#pragma once

#include "player.h"

class HumanPlayer : public Player
{
public:
	HumanPlayer(CheckerColor color);

	Command* takeTurn(CheckersGameState& gameState) override;

private:

};