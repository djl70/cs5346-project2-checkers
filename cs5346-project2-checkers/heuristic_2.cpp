#include "heuristic_2.h"

#include <limits>

int Heuristic_2::terminal(GameOverCondition condition, CheckerColor playerColor) const
{
	switch (condition)
	{
	case kBlackCannotMove:
	case kBlackHasNoPiecesLeft:
		return playerColor == kBlack ? -std::numeric_limits<int>::max() + 1 : std::numeric_limits<int>::max() - 1;
	case kRedCannotMove:
	case kRedHasNoPiecesLeft:
		return playerColor == kRed ? -std::numeric_limits<int>::max() + 1 : std::numeric_limits<int>::max() - 1;
	case kTurnLimitReached:
	case kBoardStateRepetitionLimitReached:
	default:
		return 0;
	}
}

int Heuristic_2::value(const checkerboard::Checkerboard& board) const
{
	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	// First, check if the game is over for the current board state
	// Victory = max value
	// Loss = min value
	// Draw = 0
	GameOverCondition gameOverCondition;
	if (checkerboard::isGameOver(board, gameOverCondition))
	{
		return terminal(gameOverCondition, playerColor);
	}

	// Heuristic used by https://github.com/vikrambombhi/Checkers/blob/master/src/AI.cpp
	// Value breakdown:
	// +40 for each king
	// +20 for each man
	// +1 for each piece in the center 4 squares
	// +2 for each piece in the ring of squares around the center
	// +3 for each piece in the ring of squares around that ring
	// +4 for each piece in the outer ring of squares (i.e., the edges)
	// Determine for opponent, too, and get difference

	int myValue = 0;
	int otherValue = 0;
	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			if ((r + c) % 2 == 1)
			{
				const CheckerSquare& square = board.board.at(checkerboard::index({ c, r }));
				if (square.isEmpty())
				{
					continue;
				}

				const CheckerPiece& piece = board.pieces.at(square.getPieceIndex());
				int squareOwner = piece.getColor() == playerColor ? board.currentPlayer : checkerboard::nextPlayer(board.currentPlayer);
				CheckerColor squareOwnerColor = squareOwner == 0 ? kBlack : kRed;
				int* value = squareOwnerColor == playerColor ? &myValue : &otherValue;

				if (piece.isKing())
				{
					*value += 40;
				}
				else
				{
					*value += 20;
				}

				// Check which ring of squares the piece is in
				if (r == 0 || r == 7 || c == 0 || c == 7)
				{
					*value += 4;
				}
				else if (r == 1 || r == 6 || c == 1 || c == 6)
				{
					*value += 3;
				}
				else if (r == 2 || r == 5 || c == 2 || c == 5)
				{
					*value += 2;
				}
				else // r == 3 || r == 4 || c == 3 || c == 4
				{
					*value += 1;
				}
			}
		}
	}

	return myValue - otherValue;
}