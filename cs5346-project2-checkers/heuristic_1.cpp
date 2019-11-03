#include "heuristic_1.h"

int Heuristic_1::value(const checkerboard::Checkerboard& board) const
{
	// Value breakdown:
	// +1 for each friendly neighboring piece around the current piece
	// +50 for each man
	// +100 for each king
	// +10 for each piece on an edge (+20 if on the row closest to the player)
	// Determine for opponent, too, and get difference

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

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