#pragma once

#include "checker_piece.h"
#include "checker_square.h"
#include "jump_command.h"
#include "move_command.h"

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
//CheckerSquare* findJumpNorthWest(CheckerSquare& from, CheckerColor playerColor);
//CheckerSquare* findJumpNorthEast(CheckerSquare& from, CheckerColor playerColor);
//CheckerSquare* findJumpSouthWest(CheckerSquare& from, CheckerColor playerColor);
//CheckerSquare* findJumpSouthEast(CheckerSquare& from, CheckerColor playerColor);

std::vector<MoveInfo> findAllValidMoves(Checkerboard& board, CheckerColor playerColor);
std::vector<MoveInfo> findValidMoves(Checkerboard& square, CheckerColor playerColor, const sf::Vector2i& start);
bool findMoveInDirection(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, MoveInfo*& outJump);
//CheckerSquare* findMoveNorthWest(CheckerSquare& from, CheckerColor playerColor);
//CheckerSquare* findMoveNorthEast(CheckerSquare& from, CheckerColor playerColor);
//CheckerSquare* findMoveSouthWest(CheckerSquare& from, CheckerColor playerColor);
//CheckerSquare* findMoveSouthEast(CheckerSquare& from, CheckerColor playerColor);