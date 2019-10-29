#include "checkerboard.h"

std::vector<JumpInfo> findAllValidJumps(Checkerboard& board, CheckerColor playerColor)
{
	std::vector<JumpInfo> jumps;

	for (auto& square : board.board)
	{
		std::vector<JumpInfo> squareJumps = findValidJumps(square, playerColor);
		for (auto& info : squareJumps)
		{
			jumps.push_back(info);
		}
	}

	return jumps;
}

std::vector<JumpInfo> findValidJumps(CheckerSquare& square, CheckerColor playerColor)
{
	std::vector<JumpInfo> jumps;

	CheckerPiece* piece = square.getPiece();
	if (piece && piece->getColor() == playerColor)
	{
		// Check northern neighbors
		if (piece->getColor() == kBlack || piece->isKing())
		{
			CheckerSquare* to = findJumpNorthWest(square, playerColor);
			if (to)
			{
				jumps.push_back({ square, *to, *square.getNeighbors().pNeighborNorthWest });
			}

			to = findJumpNorthEast(square, playerColor);
			if (to)
			{
				jumps.push_back({ square, *to, *square.getNeighbors().pNeighborNorthEast });
			}
		}
		// Check southern neighbors
		if (piece->getColor() == kRed || piece->isKing())
		{
			CheckerSquare* to = findJumpSouthWest(square, playerColor);
			if (to)
			{
				jumps.push_back({ square, *to, *square.getNeighbors().pNeighborSouthWest });
			}

			to = findJumpSouthEast(square, playerColor);
			if (to)
			{
				jumps.push_back({ square, *to, *square.getNeighbors().pNeighborSouthEast });
			}
		}
	}

	return jumps;
}

CheckerSquare* findJumpNorthWest(CheckerSquare& from, CheckerColor playerColor)
{
	NeighboringSquares neighbors = from.getNeighbors();

	if (neighbors.pNeighborNorthWest)
	{
		CheckerPiece* neighborPiece = neighbors.pNeighborNorthWest->getPiece();
		// If there is an opponent's piece in the adjacent square, check if we can jump it
		if (neighborPiece && neighborPiece->getColor() != playerColor)
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

CheckerSquare* findJumpNorthEast(CheckerSquare& from, CheckerColor playerColor)
{
	NeighboringSquares neighbors = from.getNeighbors();

	if (neighbors.pNeighborNorthEast)
	{
		CheckerPiece* neighborPiece = neighbors.pNeighborNorthEast->getPiece();
		// If there is an opponent's piece in the adjacent square, check if we can jump it
		if (neighborPiece && neighborPiece->getColor() != playerColor)
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

CheckerSquare* findJumpSouthWest(CheckerSquare& from, CheckerColor playerColor)
{
	NeighboringSquares neighbors = from.getNeighbors();

	if (neighbors.pNeighborSouthWest)
	{
		CheckerPiece* neighborPiece = neighbors.pNeighborSouthWest->getPiece();
		// If there is an opponent's piece in the adjacent square, check if we can jump it
		if (neighborPiece && neighborPiece->getColor() != playerColor)
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

CheckerSquare* findJumpSouthEast(CheckerSquare& from, CheckerColor playerColor)
{
	NeighboringSquares neighbors = from.getNeighbors();

	if (neighbors.pNeighborSouthEast)
	{
		CheckerPiece* neighborPiece = neighbors.pNeighborSouthEast->getPiece();
		// If there is an opponent's piece in the adjacent square, check if we can jump it
		if (neighborPiece && neighborPiece->getColor() != playerColor)
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
		// Check northern neighbors
		if (piece->getColor() == kBlack || piece->isKing())
		{
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
		// Check southern neighbors
		if (piece->getColor() == kRed || piece->isKing())
		{
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
	}

	return nullptr;
}