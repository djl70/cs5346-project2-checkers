#include "checkerboard.h"

namespace checkerboard
{
	int index(const sf::Vector2i& coord)
	{
		return coord.y * 8 + coord.x;
	}

	void placePieceAt(Checkerboard& board, int pieceIndex, int toIndex)
	{
		CheckerSquare& square = board.board.at(toIndex);
		CheckerPiece& piece = board.pieces.at(pieceIndex);

		piece.setPosition(square.getPositionInWindow());
		square.setPieceIndex(pieceIndex);
		if (square.promotesColor(piece.getColor()))
		{
			piece.promote();
		}
	}

	void movePieceFromTo(Checkerboard& board, int pieceIndex, int fromIndex, int toIndex)
	{
		CheckerSquare& from = board.board.at(fromIndex);
		from.setPieceIndex(-1);

		placePieceAt(board, pieceIndex, toIndex);
	}

	int capturePieceFrom(Checkerboard& board, int pieceIndex, int fromIndex)
	{
		CheckerSquare& from = board.board.at(fromIndex);
		from.setPieceIndex(-1);

		CheckerPiece& piece = board.pieces.at(pieceIndex);

		int capturedIndex = -1;
		std::vector<CheckerSquare>& capturedArea = piece.getColor() == kBlack ? board.capturedBlackSquares : board.capturedRedSquares;
		for (int i = 0; i < capturedArea.size(); ++i)
		{
			if (capturedArea.at(i).isEmpty())
			{
				capturedIndex = i;
				CheckerSquare& toCaptured = capturedArea.at(capturedIndex);
				piece.setPosition(toCaptured.getPositionInWindow());
				toCaptured.setPieceIndex(pieceIndex);
				break;
			}
		}

		return capturedIndex;
	}

	int releasePieceTo(Checkerboard& board, CheckerColor color, int fromCapturedIndex, int toIndex)
	{
		std::vector<CheckerSquare>& capturedArea = color == kBlack ? board.capturedBlackSquares : board.capturedRedSquares;
		CheckerSquare& fromCaptured = capturedArea.at(fromCapturedIndex);
		int pieceIndex = fromCaptured.getPieceIndex();
		fromCaptured.setPieceIndex(-1);

		CheckerPiece& piece = board.pieces.at(pieceIndex);
		CheckerSquare& to = board.board.at(toIndex);

		piece.setPosition(to.getPositionInWindow());
		to.setPieceIndex(pieceIndex);

		return pieceIndex;
	}

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

	std::vector<JumpInfo> findAllValidJumps(const Checkerboard& board, CheckerColor playerColor)
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

	std::vector<JumpInfo> findValidJumps(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start)
	{
		std::vector<JumpInfo> jumps;

		const CheckerSquare& square = board.board.at(checkerboard::index(start));
		const CheckerPiece* piece = square.isEmpty() ? nullptr : &board.pieces.at(square.getPieceIndex());
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

	bool findJumpInDirection(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, JumpInfo*& outJump)
	{
		const CheckerSquare& from = board.board.at(checkerboard::index(start));

		sf::Vector2i jumpedIndex = start + offset;
		if (jumpedIndex.x >= 0 && jumpedIndex.x < 8 && jumpedIndex.y >= 0 && jumpedIndex.y < 8)
		{
			const CheckerSquare& jumped = board.board.at(checkerboard::index(jumpedIndex));
			const CheckerPiece* neighborPiece = jumped.isEmpty() ? nullptr : &board.pieces.at(jumped.getPieceIndex());

			// If there is an opponent's piece in the adjacent square, check if we can jump it
			if (neighborPiece && neighborPiece->getColor() != playerColor)
			{
				sf::Vector2i toIndex = start + offset + offset;
				if (toIndex.x >= 0 && toIndex.x < 8 && toIndex.y >= 0 && toIndex.y < 8)
				{
					const CheckerSquare& to = board.board.at(checkerboard::index(toIndex));

					if (to.isEmpty())
					{
						outJump = new JumpInfo{ board, start, toIndex, jumpedIndex };
						return true;
					}
				}
			}
		}

		return false;
	}

	std::vector<MoveInfo> findAllValidMoves(const Checkerboard& board, CheckerColor playerColor)
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

	std::vector<MoveInfo> findValidMoves(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start)
	{
		std::vector<MoveInfo> moves;

		const CheckerSquare& square = board.board.at(checkerboard::index(start));
		const CheckerPiece* piece = square.isEmpty() ? nullptr : &board.pieces.at(square.getPieceIndex());
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

	bool findMoveInDirection(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, MoveInfo*& outMove)
	{
		const CheckerSquare& from = board.board.at(checkerboard::index(start));

		sf::Vector2i toIndex = start + offset;
		if (toIndex.x >= 0 && toIndex.x < 8 && toIndex.y >= 0 && toIndex.y < 8)
		{
			const CheckerSquare& to = board.board.at(checkerboard::index(toIndex));

			if (to.isEmpty())
			{
				outMove = new MoveInfo{ board, start, toIndex };
				return true;
			}
		}

		return false;
	}

	std::vector<FullMoveInfo> findAllValidFullMoves(const Checkerboard& board, CheckerColor playerColor)
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
				bool isMove = itr->jumped.empty();
				if (isMove)
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

	std::vector<FullMoveInfo> findValidFullMoves(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start)
	{
		std::vector<FullMoveInfo> fullMoves;

		const CheckerSquare& square = board.board.at(checkerboard::index(start));
		const CheckerPiece* piece = square.isEmpty() ? nullptr : &board.pieces.at(square.getPieceIndex());
		if (piece && piece->getColor() == playerColor)
		{
			std::vector<FullMoveInfo> info;

			// Check northern neighbors
			if (piece->getColor() == kBlack || piece->isKing())
			{
				// North-west
				info = findFullMovesInDirection(board, playerColor, start, { -1, -1 });
				for (auto& move : info)
				{
					fullMoves.push_back(move);
				}

				// North-east
				info = findFullMovesInDirection(board, playerColor, start, { 1, -1 });
				for (auto& move : info)
				{
					fullMoves.push_back(move);
				}
			}
			// Check southern neighbors
			if (piece->getColor() == kRed || piece->isKing())
			{
				// South-west
				info = findFullMovesInDirection(board, playerColor, start, { -1, 1 });
				for (auto& move : info)
				{
					fullMoves.push_back(move);
				}

				// South-east
				info = findFullMovesInDirection(board, playerColor, start, { 1, 1 });
				for (auto& move : info)
				{
					fullMoves.push_back(move);
				}
			}
		}

		return fullMoves;
	}

	std::vector<FullMoveInfo> findFullMovesInDirection(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset)
	{
		std::vector<FullMoveInfo> fullMoves;
		const CheckerSquare& from = board.board.at(checkerboard::index(start));

		// Try jumping
		JumpInfo* jumpInfo = nullptr;
		if (findJumpInDirection(board, playerColor, start, offset, jumpInfo))
		{
			// Since we found a jump, we need to simulate the jump and search for another jump in all directions from that, except for the direction we started from
			FullMoveInfo fullJump;
			fullJump.from = jumpInfo->from;
			fullJump.jumped.push_back(jumpInfo->jumped);
			fullJump.to.push_back(jumpInfo->to);
			fullJump.promoted = jumpInfo->promoted;

			if (fullJump.promoted)
			{
				// The piece was promoted, so this full move is finished
				fullMoves.push_back(fullJump);
			}
			else
			{
				// The piece was not promoted, so we need to check if a subsequent jump is possible. Don't check in the direction we came from (-offset.x, -offset.y)
				const Checkerboard simulated = simulateJump(board, *jumpInfo);

				std::vector<FullMoveInfo> offsetMoves;
				sf::Vector2i newStart = start + offset + offset;
				sf::Vector2i avoidDirection{ -offset.x, -offset.y };
				const CheckerSquare& newStartSquare = simulated.board.at(checkerboard::index(newStart));
				const CheckerPiece* piece = newStartSquare.isEmpty() ? nullptr : &simulated.pieces.at(newStartSquare.getPieceIndex());

				sf::Vector2i newOffset;
				if (piece->getColor() == kBlack || piece->isKing())
				{
					// North-west
					newOffset = { -1, -1 };
					if (newOffset != avoidDirection)
					{
						offsetMoves = findFullMovesInDirectionRecursive(simulated, playerColor, newStart, newOffset, fullJump);
						for (auto& move : offsetMoves)
						{
							fullMoves.push_back(move);
						}
					}

					// North-east
					newOffset = { 1, -1 };
					if (newOffset != avoidDirection)
					{
						offsetMoves = findFullMovesInDirectionRecursive(simulated, playerColor, newStart, newOffset, fullJump);
						for (auto& move : offsetMoves)
						{
							fullMoves.push_back(move);
						}
					}
				}
				if (piece->getColor() == kRed || piece->isKing())
				{
					// South-west
					newOffset = { -1, 1 };
					if (newOffset != avoidDirection)
					{
						offsetMoves = findFullMovesInDirectionRecursive(simulated, playerColor, newStart, newOffset, fullJump);
						for (auto& move : offsetMoves)
						{
							fullMoves.push_back(move);
						}
					}

					// South-east
					newOffset = { 1, 1 };
					if (newOffset != avoidDirection)
					{
						offsetMoves = findFullMovesInDirectionRecursive(simulated, playerColor, newStart, newOffset, fullJump);
						for (auto& move : offsetMoves)
						{
							fullMoves.push_back(move);
						}
					}
				}

				if (fullMoves.empty())
				{
					// No subsequent jumps were found, so add the initial jump to the vector
					fullMoves.push_back(fullJump);
				}
			}

			delete jumpInfo;
		}
		// Try moving
		else
		{
			MoveInfo* moveInfo = nullptr;
			if (findMoveInDirection(board, playerColor, start, offset, moveInfo))
			{
				// We found a move
				FullMoveInfo fullJump;
				fullJump.from = moveInfo->from;
				fullJump.to.push_back(moveInfo->to);
				fullJump.promoted = moveInfo->promoted;
				fullMoves.push_back(fullJump);

				delete moveInfo;
			}
		}

		return fullMoves;
	}

	// Return whether the move is terminal. If it is terminal, add its constructed move to the vector of moves.
	std::vector<FullMoveInfo> findFullMovesInDirectionRecursive(const Checkerboard& board, CheckerColor playerColor, const sf::Vector2i& start, const sf::Vector2i& offset, const FullMoveInfo& currentMove)
	{
		std::vector<FullMoveInfo> fullMoves;
		const CheckerSquare& from = board.board.at(checkerboard::index(start));

		// Try jumping
		JumpInfo* jumpInfo = nullptr;
		if (findJumpInDirection(board, playerColor, start, offset, jumpInfo))
		{
			// Since we found a jump, we need to simulate the jump and search for another jump in all directions from that, except for the direction we started from
			FullMoveInfo buildingMove{ currentMove };
			buildingMove.jumped.push_back(jumpInfo->jumped);
			buildingMove.to.push_back(jumpInfo->to);
			buildingMove.promoted = jumpInfo->promoted;

			if (buildingMove.promoted)
			{
				// Terminal case; we made another jump and the piece was promoted, so this full move is finished
				fullMoves.push_back(buildingMove);
			}
			else
			{
				// The piece was not promoted, so we still need to check if another jump is possible
				const Checkerboard simulated = simulateJump(board, *jumpInfo);

				std::vector<FullMoveInfo> offsetMoves;
				sf::Vector2i newStart = start + offset + offset;
				sf::Vector2i avoidDirection{ -offset.x, -offset.y };
				const CheckerSquare& newStartSquare = simulated.board.at(checkerboard::index(newStart));
				const CheckerPiece* piece = newStartSquare.isEmpty() ? nullptr : &simulated.pieces.at(newStartSquare.getPieceIndex());

				sf::Vector2i newOffset;
				if (piece->getColor() == kBlack || piece->isKing())
				{
					// North-west
					newOffset = { -1, -1 };
					if (newOffset != avoidDirection)
					{
						offsetMoves = findFullMovesInDirectionRecursive(simulated, playerColor, newStart, newOffset, buildingMove);
						for (auto& move : offsetMoves)
						{
							fullMoves.push_back(move);
						}
					}

					// North-east
					newOffset = { 1, -1 };
					if (newOffset != avoidDirection)
					{
						offsetMoves = findFullMovesInDirectionRecursive(simulated, playerColor, newStart, newOffset, buildingMove);
						for (auto& move : offsetMoves)
						{
							fullMoves.push_back(move);
						}
					}
				}
				if (piece->getColor() == kRed || piece->isKing())
				{
					// South-west
					newOffset = { -1, 1 };
					if (newOffset != avoidDirection)
					{
						offsetMoves = findFullMovesInDirectionRecursive(simulated, playerColor, newStart, newOffset, buildingMove);
						for (auto& move : offsetMoves)
						{
							fullMoves.push_back(move);
						}
					}

					// South-east
					newOffset = { 1, 1 };
					if (newOffset != avoidDirection)
					{
						offsetMoves = findFullMovesInDirectionRecursive(simulated, playerColor, newStart, newOffset, buildingMove);
						for (auto& move : offsetMoves)
						{
							fullMoves.push_back(move);
						}
					}
				}

				if (fullMoves.empty())
				{
					// Terminal case: no subsequent jumps were found, so add our jump to the vector
					fullMoves.push_back(buildingMove);
				}
			}

			delete jumpInfo;
		}
		else
		{
			// Terminal case: no other jumps are available from here
			// Do nothing here; let the caller add the move to its vector
		}

		return fullMoves;
	}
}