#include "heuristic_1.h"

#include <limits>

int Heuristic_1::terminal(GameOverCondition condition, CheckerColor playerColor) const
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

	// Value breakdown:
	// +2 per piece (man or king)
	// +1 per king if not trapped
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

				*value += 2;

				if (piece.isKing() && !isTrapped(board, { c, r }))
				{
					*value += 1;
				}
			}
		}
	}

	return myValue - otherValue;
}

bool Heuristic_1::isTrapped(const checkerboard::Checkerboard& board, const sf::Vector2i& squareCoord) const
{
	// A king is trapped if it cannot move in any direction
	return checkerboard::findValidFullMoves(board, board.currentPlayer == 0 ? kBlack : kRed, squareCoord).empty();
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