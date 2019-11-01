#include "heuristic_1.h"

int Heuristic_1::value(const checkerboard::Checkerboard& board, int currentPlayer) const
{
	// Value breakdown:
	// +1 for each friendly neighboring piece around the current piece
	// +1 for each man
	// +2 for each king
	// Determine for opponent, too, and get difference

	CheckerColor playerColor = currentPlayer == 0 ? kBlack : kRed;

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
				int* value = piece.getColor() == playerColor ? &myValue : &otherValue;

				if (piece.isKing())
				{
					*value += 2;
				}
				else
				{
					*value += 1;
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
			}
		}
	}

	return myValue - otherValue;
}