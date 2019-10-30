#include "full_move_command.h"

#include "checker_piece.h"
#include "checker_square.h"
#include "checkerboard.h"

FullMoveCommand::FullMoveCommand(Checkerboard& board, const FullMoveInfo& info)
	: m_board{ board }
	, m_info{ info }
	, m_partialExecutionStep{ -1 }
{

}

bool FullMoveCommand::executeStep()
{
	// If there are no jumped squares, perform a simple move then return true
	if (m_info.jumped.empty())
	{
		CheckerSquare& fromSquare = m_board.board.at(m_info.from.y * 8 + m_info.from.x);
		CheckerSquare& toSquare = m_board.board.at(m_info.to.at(0).y * 8 + m_info.to.at(0).x);

		CheckerPiece* fromPiece = fromSquare.getPiece();
		toSquare.setPiece(fromPiece);
		fromSquare.setPiece(nullptr);
		return true;
	}

	// Otherwise, perform a single jump (there could be multiple)
	sf::Vector2i fromCoords = m_partialExecutionStep == -1 ? m_info.from : m_info.to.at(m_partialExecutionStep);
	sf::Vector2i jumpedCoords = m_info.jumped.at(m_partialExecutionStep + 1);
	sf::Vector2i toCoords = m_info.to.at(m_partialExecutionStep + 1);

	CheckerSquare& fromSquare = m_board.board.at(fromCoords.y * 8 + fromCoords.x);
	CheckerSquare& jumpedSquare = m_board.board.at(jumpedCoords.y * 8 + jumpedCoords.x);
	CheckerSquare& toSquare = m_board.board.at(toCoords.y * 8 + toCoords.x);

	// Move the piece from <from> to <to>
	CheckerPiece* movedPiece = fromSquare.getPiece();
	toSquare.setPiece(movedPiece);
	fromSquare.setPiece(nullptr);

	// Capture the piece from <jumped>
	CheckerPiece* jumpedPiece = jumpedSquare.getPiece();
	std::vector<CheckerSquare>& capturedArea = movedPiece->getColor() == kBlack ? m_board.capturedRedSquares : m_board.capturedBlackSquares;

	// Determine the first available spot in the captured area on the first step
	if (m_partialExecutionStep == -1)
	{
		for (int i = 0; i < capturedArea.size(); ++i)
		{
			if (capturedArea.at(i).isEmpty())
			{
				m_firstCapturedIndex = i;
				break;
			}
		}
	}

	capturedArea.at(m_firstCapturedIndex + m_partialExecutionStep + 1).setPiece(jumpedPiece);
	jumpedSquare.setPiece(nullptr);

	// Move to the next step
	++m_partialExecutionStep;
	return m_partialExecutionStep == m_info.jumped.size() - 1;
}

bool FullMoveCommand::undoStep()
{
	// If there are no jumped squares, undo the simple move then return true
	if (m_info.jumped.empty())
	{
		CheckerSquare& fromSquare = m_board.board.at(m_info.from.y * 8 + m_info.from.x);
		CheckerSquare& toSquare = m_board.board.at(m_info.to.at(0).y * 8 + m_info.to.at(0).x);

		CheckerPiece* fromPiece = toSquare.getPiece();
		toSquare.setPiece(nullptr);
		fromSquare.setPiece(fromPiece);
		if (m_info.promoted)
		{
			fromPiece->demote();
		}
		return true;
	}

	// Otherwise, undo a single jump (there could be multiple)
	sf::Vector2i fromCoords = m_partialExecutionStep > 0 ? m_info.to.at(m_partialExecutionStep - 1) : m_info.from;
	sf::Vector2i jumpedCoords = m_info.jumped.at(m_partialExecutionStep);
	sf::Vector2i toCoords = m_info.to.at(m_partialExecutionStep);

	CheckerSquare& fromSquare = m_board.board.at(fromCoords.y * 8 + fromCoords.x);
	CheckerSquare& jumpedSquare = m_board.board.at(jumpedCoords.y * 8 + jumpedCoords.x);
	CheckerSquare& toSquare = m_board.board.at(toCoords.y * 8 + toCoords.x);

	// Move the piece from <to> to <from>
	CheckerPiece* movedPiece = toSquare.getPiece();
	toSquare.setPiece(nullptr);
	fromSquare.setPiece(movedPiece);

	// Return the captured piece to <jumped>
	std::vector<CheckerSquare>& capturedArea = movedPiece->getColor() == kBlack ? m_board.capturedRedSquares : m_board.capturedBlackSquares;
	CheckerPiece* jumpedPiece = capturedArea.at(m_firstCapturedIndex + m_partialExecutionStep).getPiece();
	capturedArea.at(m_firstCapturedIndex + m_partialExecutionStep).setPiece(nullptr);
	jumpedSquare.setPiece(jumpedPiece);

	// Undo promotion if necessary
	if (m_partialExecutionStep == m_info.jumped.size() - 1 && m_info.promoted)
	{
		movedPiece->demote();
	}

	// Move to the previous step
	--m_partialExecutionStep;
	return m_partialExecutionStep < 0;
}

void FullMoveCommand::execute()
{
	// Run all steps
	while (!executeStep())
	{

	}
}

void FullMoveCommand::undo()
{
	// Undo all steps
	while (!undoStep())
	{

	}
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