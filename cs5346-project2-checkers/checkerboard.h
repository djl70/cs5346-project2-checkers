#pragma once

#include "checker_piece.h"
#include "checker_square.h"
#include "move_command.h"

#include <vector>

struct Checkerboard
{
	std::vector<CheckerSquare> board;
	std::vector<CheckerSquare> capturedRedSquares;
	std::vector<CheckerSquare> capturedBlackSquares;
};

std::vector<MoveInfo> listAllValidMoves(Checkerboard& board, CheckerColor playerColor);