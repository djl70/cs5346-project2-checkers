#pragma once

/*
#include "checker_square.h"

#include <array>

class Checkerboard
{
public:
	const static size_t kRows = 8;
	const static size_t kColumns = 8;

	Checkerboard()
	{
		for (size_t r = 0; r < m_squareGrid.size(); ++r)
		{
			auto& row = m_squareGrid[r];
			for (size_t c = 0; c < row.size(); ++c)
			{
				auto& square = row[c];
				bool isBlack = (r + c) % 2 == 1;
				square = CheckerSquare(sf::FloatRect(c * 100.0f, r * 100.0f, 100.0f, 100.0f), isBlack ? CheckerColor::kBlack : CheckerColor::kWhite);
				if (isBlack)
				{
					if (r < 3)
					{
						square.placePiece(new CheckerPiece(45.0f, CheckerColor::kBlack));
					}
					else if (r > 4)
					{
						square.placePiece(new CheckerPiece(45.0f, CheckerColor::kWhite));
					}
				}
			}
		}

		m_playerTurn = kBlack;
	}

	~Checkerboard()
	{
		for (auto& row : m_squareGrid)
		{
			for (auto& square : row)
			{
				if (square.hasPiece())
				{
					delete square.takePiece();
				}
			}
		}
	}

	void dragPiece(int row, int column)
	{
		if (row >= kRows || column >= kColumns)
		{
			return;
		}

		if (m_squareGrid[row][column].hasPiece()
			&& m_squareGrid[row][column].getPieceColor() == m_playerTurn)
		{
			m_squareGrid[row][column].attachPieceToMouse();
		}
	}

	bool movePiece(int fromRow, int fromColumn, int toRow, int toColumn)
	{
		// Make sure that we are operating within the board's boundaries
		if (fromRow >= kRows || fromColumn >= kColumns)
		{
			return false;
		}
		else if (toRow >= kRows || toColumn >= kColumns)
		{
			// Since the "from" coordinates were valid, we have to detach the piece from the mouse
			if (m_squareGrid[fromRow][fromColumn].hasPiece())
			{
				m_squareGrid[fromRow][fromColumn].detachPieceFromMouse();
			}
			return false;
		}

		// Make sure that we are moving a piece and that the target square is empty
		if (!m_squareGrid[fromRow][fromColumn].hasPiece())
		{
			return false;
		}
		else if (m_squareGrid[toRow][toColumn].hasPiece())
		{
			// Since the "from" coordinates were valid, we have to detach the piece from the mouse
			m_squareGrid[fromRow][fromColumn].detachPieceFromMouse();
			return false;
		}

		// Detach the piece from the mouse so that it snaps to a square
		m_squareGrid[fromRow][fromColumn].detachPieceFromMouse();

		if (canMove(fromRow, fromColumn, toRow, toColumn))
		{
			// Transfer ownership of the piece between the "from" and "to" squares
			m_squareGrid[toRow][toColumn].placePiece(m_squareGrid[fromRow][fromColumn].takePiece());
		}
		return true;
	}

	void update(sf::RenderWindow* pWindow, const sf::Time& deltaTime)
	{
		for (auto& row : m_squareGrid)
		{
			for (auto& square : row)
			{
				square.update(pWindow, deltaTime);
			}
		}
	}

	void draw(sf::RenderWindow* pWindow) const
	{
		const CheckerSquare* pSquareWithDraggedPiece = nullptr;

		// Draw all squares and pieces, but save the dragged piece for last
		for (auto& row : m_squareGrid)
		{
			for (auto& square : row)
			{
				square.draw(pWindow);
				if (square.isPieceAttachedToMouse())
				{
					pSquareWithDraggedPiece = &square;
				}
				else
				{
					square.drawPiece(pWindow);
				}
			}
		}

		// Draw the dragged piece last so that it is drawn on top of everything else
		if (pSquareWithDraggedPiece != nullptr)
		{
			pSquareWithDraggedPiece->drawPiece(pWindow);
		}
	}

private:
	CheckerColor m_playerTurn;
	std::array<std::array<CheckerSquare, kColumns>, kRows> m_squareGrid;

	void nextTurn()
	{
		if (m_playerTurn == kBlack)
		{
			m_playerTurn = kWhite;
		}
		else
		{
			m_playerTurn = kBlack;
		}
	}

	// TODO: If a jump can be made, it's mandatory. If a man jumps into the kings row,
	// the move ends. End the game when the pieces of one color are all gone.
	// And other rules. Also, indicate which player's turn it is, and mark the
	// last piece that was moved.
	bool canMove(int fromRow, int fromColumn, int toRow, int toColumn)
	{
		// Ensure both squares exist
		if (fromRow < 0 || fromRow >= kRows
			|| toRow < 0 || toRow >= kRows
			|| fromColumn < 0 || fromColumn >= kColumns
			|| toColumn < 0 || toColumn >= kColumns)
		{
			return false;
		}

		auto& fromSquare = m_squareGrid[fromRow][fromColumn];
		auto& toSquare = m_squareGrid[toRow][toColumn];
		
		// Ensure the "from" square has a piece and is the correct color
		if (!fromSquare.hasPiece() || fromSquare.getPieceColor() != m_playerTurn)
		{
			return false;
		}

		// Force diagonal movements
		int rowChange = toRow - fromRow;
		int columnChange = toColumn - fromColumn;
		if (abs(rowChange) != abs(columnChange))
		{
			return false;
		}

		// Check if the movement would king the piece
		bool willKing = fromSquare.getPieceRank() == kMan && ((fromSquare.getPieceColor() == kWhite && toRow == 0)
			|| (fromSquare.getPieceColor() == kBlack && toRow == kRows - 1));

		// If the square is adjacent, empty, and in the right direction,
		// then allow the movement
		bool isMovingUp = rowChange < 0;
		bool isAdjacent = abs(rowChange) == 1;
		bool isMovementForward = (fromSquare.getPieceColor() == kWhite && isMovingUp) || (fromSquare.getPieceColor() == kBlack && !isMovingUp);
		bool canMoveInDirection = fromSquare.getPieceRank() == kKing || isMovementForward;
		if (isAdjacent
			&& toSquare.canPlacePiece()
			&& canMoveInDirection)
		{
			if (willKing)
			{
				fromSquare.kingPiece();
			}
			nextTurn();
			return true;
		}

		// Otherwise, if the "to" square is 2 away, and if the adjacent square
		// in that direction has a piece of the opposite color, then
		// allow the movement
		bool isTwoAway = abs(rowChange) == 2;
		auto& jumpedSquare = m_squareGrid[fromRow + rowChange / 2][fromColumn + columnChange / 2];
		bool isOppositeColor = jumpedSquare.hasPiece() && fromSquare.getPieceColor() != jumpedSquare.getPieceColor();
		if (isTwoAway
			&& toSquare.canPlacePiece()
			&& canMoveInDirection
			&& isOppositeColor)
		{
			delete jumpedSquare.takePiece();
			if (willKing)
			{
				fromSquare.kingPiece();
				// If a man jumps into the kings row, the move terminates
				nextTurn();
			}
			return true;
		}

		return false;
	}
};
*/