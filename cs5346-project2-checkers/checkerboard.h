#pragma once

#include "checker_piece.h"
#include "checker_square.h"
#include "jump_command.h"
#include "move_command.h"
#include "full_move_command.h"

#include <vector>

struct Checkerboard
{
	std::vector<CheckerSquare> board;
	std::vector<CheckerSquare> capturedRedSquares;
	std::vector<CheckerSquare> capturedBlackSquares;
};

std::vector<JumpInfo> findAllValidJumps(Checkerboard& board, CheckerColor playerColor);
std::vector<JumpInfo> findValidJumps(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start);
bool findJumpInDirection(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, JumpInfo*& outJump);

std::vector<MoveInfo> findAllValidMoves(Checkerboard& board, CheckerColor playerColor);
std::vector<MoveInfo> findValidMoves(Checkerboard& square, CheckerColor playerColor, const sf::Vector2i& start);
bool findMoveInDirection(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, MoveInfo*& outMove);

std::vector<FullMoveInfo> findAllValidFullMoves(Checkerboard& board, CheckerColor playerColor);
std::vector<FullMoveInfo> findValidFullMoves(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start);
bool findFullMoveInDirection(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, FullMoveInfo*& outFullMove);