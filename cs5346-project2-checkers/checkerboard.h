#pragma once

#include "checker_piece.h"
#include "checker_square.h"
#include "jump_command.h"
#include "move_command.h"
#include "full_move_command.h"

#include <bitset>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

namespace checkerboard
{
	const unsigned int kBitsToEncodeBoardState = 97; // 1 bit for player, 32 bits for occupied (yes/no), 32 bits for piece color (red/black), 32 bits for piece rank (man/king)

	struct Checkerboard
	{
		std::vector<CheckerSquare> board;
		std::vector<CheckerSquare> capturedRedSquares;
		std::vector<CheckerSquare> capturedBlackSquares;
		std::vector<CheckerPiece> pieces;
		int turnNumber = 0;
		int currentPlayer = 0;
		int numTurnsSinceCaptureOrKinging = 0;
		std::unordered_map<std::bitset<checkerboard::kBitsToEncodeBoardState>, int> boardStateFrequency;
	};

	int index(const sf::Vector2i& coord);
	void placePieceAt(Checkerboard& board, int pieceIndex, int toIndex);
	void movePieceFromTo(Checkerboard& board, int pieceIndex, int fromIndex, int toIndex);
	int capturePieceFrom(Checkerboard& board, int pieceIndex, int fromIndex);
	int releasePieceTo(Checkerboard& board, CheckerColor color, int fromCapturedIndex, int toIndex);

	int nextPlayer(int currentPlayer);

	std::bitset<kBitsToEncodeBoardState> encode(const Checkerboard& board);

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