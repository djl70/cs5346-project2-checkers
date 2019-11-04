#include "heuristic_1.h"

#include <limits>

int Heuristic_1::terminal(GameOverCondition condition, CheckerColor playerColor) const
{
	switch (condition)
	{
	case kBlackCannotMove:
	case kBlackHasNoPiecesLeft:
		return playerColor == kBlack ? -std::numeric_limits<int>::max() : std::numeric_limits<int>::max();
	case kRedCannotMove:
	case kRedHasNoPiecesLeft:
		return playerColor == kRed ? -std::numeric_limits<int>::max() : std::numeric_limits<int>::max();
	case kTurnLimitReached:
	case kBoardStateRepetitionLimitReached:
		return 0;
	}
}

int Heuristic_1::earlyGame(const checkerboard::Checkerboard& board) const
{
	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	// Value breakdown:
	// +1 for each friendly neighboring piece around the current piece
	// +50 for each man
	// +100 for each king
	// +10 for each piece on an edge (+20 if on the row closest to the player)
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
					*value += 100;
				}
				else
				{
					*value += 50;
				}

				// Check neighboring squares
				int index = checkerboard::index({ c - 1, r + 1 });
				if (index >= 0 && index < 64)
				{
					const CheckerSquare* neighbor = &board.board.at(index);
					if (!neighbor->isEmpty() && board.pieces.at(neighbor->getPieceIndex()).getColor() == piece.getColor())
					{
						*value += 1;
					}
				}

				index = checkerboard::index({ c + 1, r + 1 });
				if (index >= 0 && index < 64)
				{
					const CheckerSquare* neighbor = &board.board.at(index);
					if (!neighbor->isEmpty() && board.pieces.at(neighbor->getPieceIndex()).getColor() == piece.getColor())
					{
						*value += 1;
					}
				}

				index = checkerboard::index({ c - 1, r - 1 });
				if (index >= 0 && index < 64)
				{
					const CheckerSquare* neighbor = &board.board.at(index);
					if (!neighbor->isEmpty() && board.pieces.at(neighbor->getPieceIndex()).getColor() == piece.getColor())
					{
						*value += 1;
					}
				}

				index = checkerboard::index({ c + 1, r - 1 });
				if (index >= 0 && index < 64)
				{
					const CheckerSquare* neighbor = &board.board.at(index);
					if (!neighbor->isEmpty() && board.pieces.at(neighbor->getPieceIndex()).getColor() == piece.getColor())
					{
						*value += 1;
					}
				}

				// Check if on an edge
				if (r == 0 || r == 7 || c == 0 || c == 7)
				{
					// Check if on the row closest to the player
					if ((r == 0 && squareOwnerColor == kRed) || (r == 7 && squareOwnerColor == kBlack))
					{
						*value += 20;
					}
					else
					{
						*value += 10;
					}
				}
			}
		}
	}

	return myValue - otherValue;
}

int Heuristic_1::lateGame(const checkerboard::Checkerboard& board) const
{
	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	// TODO: For now, just do the same thing as the early game. But this should be changed to a more specialized heuristic for the late game.
	return earlyGame(board);
}

int Heuristic_1::value(const checkerboard::Checkerboard& board) const
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

	// If the game is not over, value the board differently depending on the turn number
	if (board.turnNumber < 50)
	{
		return earlyGame(board);
	}
	else
	{
		return lateGame(board);
	}
}