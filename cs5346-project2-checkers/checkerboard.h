#pragma once

#include "checker_piece.h"
#include "checker_square.h"
#include "jump_command.h"
#include "move_command.h"
#include "full_move_command.h"

#include <vector>

#include <SFML/Graphics.hpp>

namespace checkerboard
{
	struct Checkerboard
	{
		std::vector<CheckerSquare> board;
		std::vector<CheckerSquare> capturedRedSquares;
		std::vector<CheckerSquare> capturedBlackSquares;
		std::vector<CheckerPiece> pieces;
	};

	int index(const sf::Vector2i& coord);
	void placePieceAt(Checkerboard& board, int pieceIndex, int toIndex);
	void movePieceFromTo(Checkerboard& board, int pieceIndex, int fromIndex, int toIndex);
	int capturePieceFrom(Checkerboard& board, int pieceIndex, int fromIndex);
	int releasePieceTo(Checkerboard& board, CheckerColor color, int fromCapturedIndex, int toIndex);

	Checkerboard simulateMove(const Checkerboard& board, const MoveInfo& info);
	Checkerboard simulateJump(const Checkerboard& board, const JumpInfo& info);

	std::vector<JumpInfo> findAllValidJumps(const Checkerboard& board, CheckerColor playerColor);
	std::vector<JumpInfo> findValidJumps(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start);
	bool findJumpInDirection(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, JumpInfo*& outJump);

	std::vector<MoveInfo> findAllValidMoves(const Checkerboard& board, CheckerColor playerColor);
	std::vector<MoveInfo> findValidMoves(const Checkerboard& square, CheckerColor playerColor, const sf::Vector2i& start);
	bool findMoveInDirection(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, MoveInfo*& outMove);

	std::vector<FullMoveInfo> findAllValidFullMoves(const Checkerboard& board, CheckerColor playerColor);
	std::vector<FullMoveInfo> findValidFullMoves(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start);
	std::vector<FullMoveInfo> findFullMovesInDirection(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset);
	std::vector<FullMoveInfo> findFullMovesInDirectionRecursive(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, const FullMoveInfo& currentMove);
}