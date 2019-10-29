#include "checkerboard.h"

std::vector<MoveInfo> findAllValidMoves(Checkerboard& board, CheckerColor playerColor)
{
	std::vector<MoveInfo> moves;

	for (auto& square : board.board)
	{
		std::vector<MoveInfo> squareMoves = findValidMoves(square, playerColor);
		for (auto& info : squareMoves)
		{
			moves.push_back(info);
		}
	}

	return moves;
}

std::vector<MoveInfo> findValidMoves(CheckerSquare& square, CheckerColor playerColor)
{
	std::vector<MoveInfo> moves;

	CheckerPiece* piece = square.getPiece();
	if (piece && piece->getColor() == playerColor)
	{
		if (piece->isKing())
		{
			// Can move to all neighbors
			CheckerSquare* to = findMoveNorthWest(square, playerColor);
			if (to)
			{
				moves.push_back({ square, *to });
			}

			to = findMoveNorthEast(square, playerColor);
			if (to)
			{
				moves.push_back({ square, *to });
			}

			to = findMoveSouthWest(square, playerColor);
			if (to)
			{
				moves.push_back({ square, *to });
			}

			to = findMoveSouthEast(square, playerColor);
			if (to)
			{
				moves.push_back({ square, *to });
			}
		}
		else if (piece->getColor() == kRed)
		{
			// Can only move to southern neighbors
			CheckerSquare* to = findMoveSouthWest(square, playerColor);
			if (to)
			{
				moves.push_back({ square, *to });
			}

			to = findMoveSouthEast(square, playerColor);
			if (to)
			{
				moves.push_back({ square, *to });
			}
		}
		else if (piece->getColor() == kBlack)
		{
			// Can only move to northern neighbors
			CheckerSquare* to = findMoveNorthWest(square, playerColor);
			if (to)
			{
				moves.push_back({ square, *to });
			}

			to = findMoveNorthEast(square, playerColor);
			if (to)
			{
				moves.push_back({ square, *to });
			}
		}
	}

	return moves;
}

CheckerSquare* findMoveNorthWest(CheckerSquare& from, CheckerColor playerColor)
{
	NeighboringSquares neighbors = from.getNeighbors();

	if (neighbors.pNeighborNorthWest)
	{
		CheckerPiece* neighborPiece = neighbors.pNeighborNorthWest->getPiece();

		// If there is no piece in the adjacent square, we can move there
		if (!neighborPiece)
		{
			return neighbors.pNeighborNorthWest;
		}
		// If there is an opponent's piece in the adjacent square, check if we can jump it
		else if (neighborPiece->getColor() != playerColor)
		{
			NeighboringSquares secondNeighbors = neighbors.pNeighborNorthWest->getNeighbors();
			if (secondNeighbors.pNeighborNorthWest && !secondNeighbors.pNeighborNorthWest->getPiece())
			{
				return secondNeighbors.pNeighborNorthWest;
			}
		}
	}

	return nullptr;
}

CheckerSquare* findMoveNorthEast(CheckerSquare& from, CheckerColor playerColor)
{
	NeighboringSquares neighbors = from.getNeighbors();

	if (neighbors.pNeighborNorthEast)
	{
		CheckerPiece* neighborPiece = neighbors.pNeighborNorthEast->getPiece();

		// If there is no piece in the adjacent square, we can move there
		if (!neighborPiece)
		{
			return neighbors.pNeighborNorthEast;
		}
		// If there is an opponent's piece in the adjacent square, check if we can jump it
		else if (neighborPiece->getColor() != playerColor)
		{
			NeighboringSquares secondNeighbors = neighbors.pNeighborNorthEast->getNeighbors();
			if (secondNeighbors.pNeighborNorthEast && !secondNeighbors.pNeighborNorthEast->getPiece())
			{
				return secondNeighbors.pNeighborNorthEast;
			}
		}
	}

	return nullptr;
}

CheckerSquare* findMoveSouthWest(CheckerSquare& from, CheckerColor playerColor)
{
	NeighboringSquares neighbors = from.getNeighbors();

	if (neighbors.pNeighborSouthWest)
	{
		CheckerPiece* neighborPiece = neighbors.pNeighborSouthWest->getPiece();

		// If there is no piece in the adjacent square, we can move there
		if (!neighborPiece)
		{
			return neighbors.pNeighborSouthWest;
		}
		// If there is an opponent's piece in the adjacent square, check if we can jump it
		else if (neighborPiece->getColor() != playerColor)
		{
			NeighboringSquares secondNeighbors = neighbors.pNeighborSouthWest->getNeighbors();
			if (secondNeighbors.pNeighborSouthWest && !secondNeighbors.pNeighborSouthWest->getPiece())
			{
				return secondNeighbors.pNeighborSouthWest;
			}
		}
	}

	return nullptr;
}

CheckerSquare* findMoveSouthEast(CheckerSquare& from, CheckerColor playerColor)
{
	NeighboringSquares neighbors = from.getNeighbors();

	if (neighbors.pNeighborSouthEast)
	{
		CheckerPiece* neighborPiece = neighbors.pNeighborSouthEast->getPiece();

		// If there is no piece in the adjacent square, we can move there
		if (!neighborPiece)
		{
			return neighbors.pNeighborSouthEast;
		}
		// If there is an opponent's piece in the adjacent square, check if we can jump it
		else if (neighborPiece->getColor() != playerColor)
		{
			NeighboringSquares secondNeighbors = neighbors.pNeighborSouthEast->getNeighbors();
			if (secondNeighbors.pNeighborSouthEast && !secondNeighbors.pNeighborSouthEast->getPiece())
			{
				return secondNeighbors.pNeighborSouthEast;
			}
		}
	}

	return nullptr;
}