#pragma once

#include "checkers_game_state.h"

class Command;

class Player
{
public:
	Player(CheckerColor color);

	virtual Command* takeTurn(CheckersGameState& gameState) = 0;

protected:
	CheckerColor m_color;
};