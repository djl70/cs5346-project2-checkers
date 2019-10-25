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
	std::vector<CheckerPiece> pieces;
	std::vector<CheckerSquare> squares;
};

// Initial game state:
//
// . R . R . R . R
// R . R . R . R .
// . R . R . R . R
// X . X . X . X .
// . X . X . X . X
// B . B . B . B .
// . B . B . B . B
// B . B . B . B .
//
// R = red player's piece
// B = black player's piece
// X = empty black square
// . = empty red square (ignored)
//
// The black squares are indexed as such:
//
//  .  0  .  1  .  2  .  3
//  4  .  5  .  6  .  7  .
//  .  8  .  9  . 10  . 11
// 12  . 13  . 14  . 15  .
//  . 16  . 17  . 18  . 19
// 20  . 21  . 22  . 23  .
//  . 24  . 25  . 26  . 27
// 28  . 29  . 30  . 31  .