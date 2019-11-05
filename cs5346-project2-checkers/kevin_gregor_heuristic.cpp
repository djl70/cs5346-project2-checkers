#include "kevin_gregor_heuristic.h"

int KevinGregorHeuristic::value(const checkerboard::Checkerboard& board) const
{
	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	GameOverCondition gameOverCondition;
	if (checkerboard::isGameOver(board, gameOverCondition))
	{
		return terminal(gameOverCondition, playerColor);
	}

	// Heuristic from https://github.com/kevingregor/Checkers
	// Values are calculated for both players and the difference is returned

	const int weightMen              =  500;
	const int weightKings            =  775;
	const int weightBackRow          =  400;
	const int weightMiddleBox        =  250;
	const int weightMiddleRows       =   50;
	const int weightVulnerablePieces = -300;
	const int weightProtectedPieces  =  300;

	int value = 0;

	value += weightMen * countMen(board);
	value += weightKings * countKings(board);
	value += weightBackRow * countBackRow(board);
	value += weightMiddleBox * countMiddleBox(board);
	value += weightMiddleRows * countMiddleRows(board);
	value += weightVulnerablePieces * countVulnerablePieces(board);
	value += weightProtectedPieces * countProtectedPieces(board);

	return value;
}

int KevinGregorHeuristic::terminal(GameOverCondition condition, CheckerColor playerColor) const
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

int KevinGregorHeuristic::countMen(const checkerboard::Checkerboard& board) const
{
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

				if (!piece.isKing())
				{
					++(*value);
				}
			}
		}
	}

	return myValue - otherValue;
}

int KevinGregorHeuristic::countKings(const checkerboard::Checkerboard& board) const
{
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
					++(*value);
				}
			}
		}
	}

	return myValue - otherValue;
}

int KevinGregorHeuristic::countBackRow(const checkerboard::Checkerboard& board) const
{
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

				if (r == 0 && squareOwnerColor == kRed)
				{
					++(*value);
				}
				else if (r == 7 && squareOwnerColor == kBlack)
				{
					++(*value);
				}
			}
		}
	}

	return myValue - otherValue;
}

int KevinGregorHeuristic::countMiddleBox(const checkerboard::Checkerboard& board) const
{
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

				if ((r == 3 || r == 4) && (c >= 2 && c <= 5))
				{
					++(*value);
				}
			}
		}
	}

	return myValue - otherValue;
}

int KevinGregorHeuristic::countMiddleRows(const checkerboard::Checkerboard& board) const
{
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

				if ((r == 3 || r == 4) && (c < 2 || c > 5))
				{
					++(*value);
				}
			}
		}
	}

	return myValue - otherValue;
}

int KevinGregorHeuristic::countVulnerablePieces(const checkerboard::Checkerboard& board) const
{
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

				bool isVulnerable = false;

				// Check northwest and southeast neighbors
				const sf::Vector2i coordsNW = { c - 1, r - 1 };
				const sf::Vector2i coordsSE = { c + 1, r + 1 };
				if (coordsNW.x >= 0 && coordsNW.x < 8 && coordsNW.y >= 0 && coordsNW.y < 8
					&& coordsSE.x >= 0 && coordsSE.x < 8 && coordsSE.y >= 0 && coordsSE.y < 8)
				{
					const CheckerSquare& neighborNW = board.board.at(checkerboard::index(coordsNW));
					const CheckerSquare& neighborSE = board.board.at(checkerboard::index(coordsSE));

					if (neighborNW.isEmpty() && !neighborSE.isEmpty())
					{
						// Northwest square is empty but southeast is not, so check if southeast is the enemy's piece
						const CheckerPiece& neighborPiece = board.pieces.at(neighborSE.getPieceIndex());
						if (neighborPiece.getColor() != squareOwnerColor)
						{
							isVulnerable = true;
						}
					}
					else if (!neighborNW.isEmpty() && neighborSE.isEmpty())
					{
						// Southeast square is empty but northwest is not, so check if northwest is the enemy's piece
						const CheckerPiece& neighborPiece = board.pieces.at(neighborNW.getPieceIndex());
						if (neighborPiece.getColor() != squareOwnerColor)
						{
							isVulnerable = true;
						}
					}
					// Otherwise, either both NW and SE are empty, or both are occupied; in either case, we are not vulnerable to them
				}

				// Check northeast and southwest neighbors
				const sf::Vector2i coordsNE = { c + 1, r - 1 };
				const sf::Vector2i coordsSW = { c - 1, r + 1 };
				if (!isVulnerable
					&& coordsNE.x >= 0 && coordsNE.x < 8 && coordsNE.y >= 0 && coordsNE.y < 8
					&& coordsSW.x >= 0 && coordsSW.x < 8 && coordsSW.y >= 0 && coordsSW.y < 8)
				{
					const CheckerSquare& neighborNE = board.board.at(checkerboard::index(coordsNE));
					const CheckerSquare& neighborSW = board.board.at(checkerboard::index(coordsSW));

					if (neighborNE.isEmpty() && !neighborSW.isEmpty())
					{
						// Northwest square is empty but southeast is not, so check if southeast is the enemy's piece
						const CheckerPiece& neighborPiece = board.pieces.at(neighborSW.getPieceIndex());
						if (neighborPiece.getColor() != squareOwnerColor)
						{
							isVulnerable = true;
						}
					}
					else if (!neighborNE.isEmpty() && neighborSW.isEmpty())
					{
						// Southeast square is empty but northwest is not, so check if northwest is the enemy's piece
						const CheckerPiece& neighborPiece = board.pieces.at(neighborNE.getPieceIndex());
						if (neighborPiece.getColor() != squareOwnerColor)
						{
							isVulnerable = true;
						}
					}
					// Otherwise, either both NE and SW are empty, or both are occupied; in either case, we are not vulnerable to them
				}

				if (isVulnerable)
				{
					++(*value);
				}
			}
		}
	}

	return myValue - otherValue;
}

int KevinGregorHeuristic::countProtectedPieces(const checkerboard::Checkerboard& board) const
{
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

				bool isVulnerable = false;

				// Check northwest and southeast neighbors
				const sf::Vector2i coordsNW = { c - 1, r - 1 };
				const sf::Vector2i coordsSE = { c + 1, r + 1 };
				if (coordsNW.x >= 0 && coordsNW.x < 8 && coordsNW.y >= 0 && coordsNW.y < 8
					&& coordsSE.x >= 0 && coordsSE.x < 8 && coordsSE.y >= 0 && coordsSE.y < 8)
				{
					const CheckerSquare& neighborNW = board.board.at(checkerboard::index(coordsNW));
					const CheckerSquare& neighborSE = board.board.at(checkerboard::index(coordsSE));

					if (neighborNW.isEmpty() && !neighborSE.isEmpty())
					{
						// Northwest square is empty but southeast is not, so check if southeast is the enemy's piece
						const CheckerPiece& neighborPiece = board.pieces.at(neighborSE.getPieceIndex());
						if (neighborPiece.getColor() != squareOwnerColor)
						{
							isVulnerable = true;
						}
					}
					else if (!neighborNW.isEmpty() && neighborSE.isEmpty())
					{
						// Southeast square is empty but northwest is not, so check if northwest is the enemy's piece
						const CheckerPiece& neighborPiece = board.pieces.at(neighborNW.getPieceIndex());
						if (neighborPiece.getColor() != squareOwnerColor)
						{
							isVulnerable = true;
						}
					}
					// Otherwise, either both NW and SE are empty, or both are occupied; in either case, we are not vulnerable to them
				}

				// Check northeast and southwest neighbors
				const sf::Vector2i coordsNE = { c + 1, r - 1 };
				const sf::Vector2i coordsSW = { c - 1, r + 1 };
				if (!isVulnerable
					&& coordsNE.x >= 0 && coordsNE.x < 8 && coordsNE.y >= 0 && coordsNE.y < 8
					&& coordsSW.x >= 0 && coordsSW.x < 8 && coordsSW.y >= 0 && coordsSW.y < 8)
				{
					const CheckerSquare& neighborNE = board.board.at(checkerboard::index(coordsNE));
					const CheckerSquare& neighborSW = board.board.at(checkerboard::index(coordsSW));

					if (neighborNE.isEmpty() && !neighborSW.isEmpty())
					{
						// Northwest square is empty but southeast is not, so check if southeast is the enemy's piece
						const CheckerPiece& neighborPiece = board.pieces.at(neighborSW.getPieceIndex());
						if (neighborPiece.getColor() != squareOwnerColor)
						{
							isVulnerable = true;
						}
					}
					else if (!neighborNE.isEmpty() && neighborSW.isEmpty())
					{
						// Southeast square is empty but northwest is not, so check if northwest is the enemy's piece
						const CheckerPiece& neighborPiece = board.pieces.at(neighborNE.getPieceIndex());
						if (neighborPiece.getColor() != squareOwnerColor)
						{
							isVulnerable = true;
						}
					}
					// Otherwise, either both NE and SW are empty, or both are occupied; in either case, we are not vulnerable to them
				}

				if (!isVulnerable)
				{
					++(*value);
				}
			}
		}
	}

	return myValue - otherValue;
}