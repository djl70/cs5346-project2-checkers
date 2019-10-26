#pragma once

#include "checker_piece.h"
#include "checker_square.h"

#include <array>
#include <stack>
#include <vector>

class Player;

struct CheckersGameState
{
	std::size_t currentPlayer;
	std::array<Player*, 2> players;
	std::vector<CheckerPiece*> pieces;
	std::vector<CheckerSquare*> squares;
};