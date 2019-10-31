#include "checkerboard.h"

Checkerboard simulateMove(const Checkerboard& board, const MoveInfo& info)
{
	Checkerboard simulated{ board };

	MoveCommand command{ simulated, info };
	command.execute();

	return simulated;
}

Checkerboard simulateJump(const Checkerboard& board, const JumpInfo& info)
{
	Checkerboard simulated{ board };

	JumpCommand command{ simulated, info };
	command.execute();

	return simulated;
}

std::vector<JumpInfo> findAllValidJumps(Checkerboard& board, CheckerColor playerColor)
{
	std::vector<JumpInfo> jumps;

	for (auto& square : board.board)
	{
		std::vector<JumpInfo> squareJumps = findValidJumps(board, playerColor, square.getPositionOnBoard());
		for (auto& info : squareJumps)
		{
			jumps.push_back(info);
		}
	}

	return jumps;
}

std::vector<JumpInfo> findValidJumps(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start)
{
	std::vector<JumpInfo> jumps;

	CheckerSquare& square = board.board[start.y * 8 + start.x];
	CheckerPiece* piece = square.getPiece();
	if (piece && piece->getColor() == playerColor)
	{
		JumpInfo* info = nullptr;
		// Check northern neighbors
		if (piece->getColor() == kBlack || piece->isKing())
		{
			// North-west
			if (findJumpInDirection(board, playerColor, start, { -1, -1 }, info))
			{
				jumps.push_back(*info);
				delete info;
			}
			// North-east
			if (findJumpInDirection(board, playerColor, start, { 1, -1 }, info))
			{
				jumps.push_back(*info);
				delete info;
			}
		}
		// Check southern neighbors
		if (piece->getColor() == kRed || piece->isKing())
		{
			// South-west
			if (findJumpInDirection(board, playerColor, start, { -1, 1 }, info))
			{
				jumps.push_back(*info);
				delete info;
			}
			// South-east
			if (findJumpInDirection(board, playerColor, start, { 1, 1 }, info))
			{
				jumps.push_back(*info);
				delete info;
			}
		}
	}

	return jumps;
}

bool findJumpInDirection(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, JumpInfo*& outJump)
{
	CheckerSquare& from = board.board[start.y * 8 + start.x];
	
	sf::Vector2i jumpedIndex = start + offset;
	if (jumpedIndex.x >= 0 && jumpedIndex.x < 8 && jumpedIndex.y >= 0 && jumpedIndex.y < 8)
	{
		CheckerSquare& jumped = board.board[jumpedIndex.y * 8 + jumpedIndex.x];
		CheckerPiece* neighborPiece = jumped.getPiece();

		// If there is an opponent's piece in the adjacent square, check if we can jump it
		if (neighborPiece && neighborPiece->getColor() != playerColor)
		{
			sf::Vector2i toIndex = start + offset + offset;
			if (toIndex.x >= 0 && toIndex.x < 8 && toIndex.y >= 0 && toIndex.y < 8)
			{
				CheckerSquare& to = board.board[toIndex.y * 8 + toIndex.x];

				if (!to.getPiece())
				{
					outJump = new JumpInfo(from, to, jumped);
					return true;
				}
			}
		}
	}

	return false;
}

std::vector<MoveInfo> findAllValidMoves(Checkerboard& board, CheckerColor playerColor)
{
	std::vector<MoveInfo> moves;

	for (auto& square : board.board)
	{
		std::vector<MoveInfo> squareMoves = findValidMoves(board, playerColor, square.getPositionOnBoard());
		for (auto& info : squareMoves)
		{
			moves.push_back(info);
		}
	}

	return moves;
}

std::vector<MoveInfo> findValidMoves(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start)
{
	std::vector<MoveInfo> moves;

	CheckerSquare& square = board.board[start.y * 8 + start.x];
	CheckerPiece* piece = square.getPiece();
	if (piece && piece->getColor() == playerColor)
	{
		MoveInfo* info = nullptr;
		// Check northern neighbors
		if (piece->getColor() == kBlack || piece->isKing())
		{
			// North-west
			if (findMoveInDirection(board, playerColor, start, { -1, -1 }, info))
			{
				moves.push_back(*info);
				delete info;
			}
			// North-east
			if (findMoveInDirection(board, playerColor, start, { 1, -1 }, info))
			{
				moves.push_back(*info);
				delete info;
			}
		}
		// Check southern neighbors
		if (piece->getColor() == kRed || piece->isKing())
		{
			// South-west
			if (findMoveInDirection(board, playerColor, start, { -1, 1 }, info))
			{
				moves.push_back(*info);
				delete info;
			}
			// South-east
			if (findMoveInDirection(board, playerColor, start, { 1, 1 }, info))
			{
				moves.push_back(*info);
				delete info;
			}
		}
	}

	return moves;
}

bool findMoveInDirection(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, MoveInfo*& outMove)
{
	CheckerSquare& from = board.board[start.y * 8 + start.x];

	sf::Vector2i toIndex = start + offset;
	if (toIndex.x >= 0 && toIndex.x < 8 && toIndex.y >= 0 && toIndex.y < 8)
	{
		CheckerSquare& to = board.board[toIndex.y * 8 + toIndex.x];

		if (!to.getPiece())
		{
			outMove = new MoveInfo(from, to);
			return true;
		}
	}

	return false;
}

std::vector<FullMoveInfo> findAllValidFullMoves(Checkerboard& board, CheckerColor playerColor)
{
	std::vector<FullMoveInfo> fullMoves;
	bool isJumpAvailable = false;

	for (auto& square : board.board)
	{
		std::vector<FullMoveInfo> squareMoves = findValidFullMoves(board, playerColor, square.getPositionOnBoard());
		for (auto& info : squareMoves)
		{
			fullMoves.push_back(info);
			isJumpAvailable |= !info.jumped.empty();
		}
	}

	// Remove all simple moves if a jump is possible
	if (isJumpAvailable)
	{
		auto itr = fullMoves.begin();
		while (itr != fullMoves.end())
		{
			bool isJump = !itr->jumped.empty();
			if (isJump)
			{
				itr = fullMoves.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

	return fullMoves;
}

std::vector<FullMoveInfo> findValidFullMoves(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start)
{
	// TODO: After making a jump, recursively check neighbors again for another possible jump with that same piece

	std::vector<FullMoveInfo> fullMoves;

	CheckerSquare& square = board.board[start.y * 8 + start.x];
	CheckerPiece* piece = square.getPiece();
	if (piece && piece->getColor() == playerColor)
	{
		FullMoveInfo* info = nullptr;

		// Check northern neighbors
		if (piece->getColor() == kBlack || piece->isKing())
		{
			// North-west
			// North-east
		}
		// Check southern neighbors
		if (piece->getColor() == kRed || piece->isKing())
		{
			// South-west
			// South-east
		}
	}

	return fullMoves;
}

bool findFullMoveInDirection(Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, FullMoveInfo*& outFullMove)
{
	CheckerSquare& from = board.board[start.y * 8 + start.x];

	// Try jumping

	// Try moving

	return false;
}