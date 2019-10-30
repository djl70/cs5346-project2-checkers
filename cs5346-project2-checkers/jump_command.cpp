#include "jump_command.h"

#include "checkerboard.h"

JumpInfo::JumpInfo(CheckerSquare& from, CheckerSquare& to, CheckerSquare& jumped)
	: from{ from.getPositionOnBoard() }
	, to{ to.getPositionOnBoard() }
	, jumped{ jumped.getPositionOnBoard() }
	, promoted{ from.getPiece() && !from.getPiece()->isKing() && to.promotesColor(from.getPiece()->getColor()) }
{

}

JumpCommand::JumpCommand(Checkerboard& board, const JumpInfo& info)
	: m_board{ board }
	, m_info{ info }
	, m_capturedSquare{ nullptr }
{

}

void JumpCommand::execute()
{
	CheckerSquare& fromSquare = m_board.board.at(m_info.from.y * 8 + m_info.from.x);
	CheckerSquare& jumpedSquare = m_board.board.at(m_info.jumped.y * 8 + m_info.jumped.x);
	CheckerSquare& toSquare = m_board.board.at(m_info.to.y * 8 + m_info.to.x);

	CheckerPiece* fromPiece = fromSquare.getPiece();
	if (fromPiece)
	{
		CheckerPiece* toPiece = toSquare.getPiece();
		if (toPiece)
		{
			throw ("Error: Jumped piece onto occupied square");
		}
		toSquare.setPiece(fromPiece);
		fromSquare.setPiece(nullptr);

		CheckerPiece* jumpedPiece = jumpedSquare.getPiece();
		if (!jumpedPiece)
		{
			throw ("Error: No piece to jump over");
		}
		else if (jumpedPiece->getColor() == fromPiece->getColor())
		{
			throw ("Error: Cannot jump over same-colored piece");
		}
		
		std::vector<CheckerSquare>& capturedArea = jumpedPiece->getColor() == kRed ? m_board.capturedRedSquares : m_board.capturedBlackSquares;
		for (auto& square : capturedArea)
		{
			if (square.isEmpty())
			{
				square.setPiece(jumpedPiece);
				jumpedSquare.setPiece(nullptr);
				m_capturedSquare = &square;
				break;
			}
		}
		if (!m_capturedSquare)
		{
			throw ("Error: No more space in the captured area");
		}
	}
	else
	{
		throw ("Error: No piece to jump with");
	}
}

void JumpCommand::undo()
{
	CheckerSquare& fromSquare = m_board.board.at(m_info.from.y * 8 + m_info.from.x);
	CheckerSquare& jumpedSquare = m_board.board.at(m_info.jumped.y * 8 + m_info.jumped.x);
	CheckerSquare& toSquare = m_board.board.at(m_info.to.y * 8 + m_info.to.x);

	// We will assume that <from> had a piece, <to> did not, and <jumped> had its piece moved to <m_capturedSquare>
	CheckerPiece* fromPiece = toSquare.getPiece();
	fromSquare.setPiece(fromPiece);
	toSquare.setPiece(nullptr);
	CheckerPiece* jumpedPiece = m_capturedSquare->getPiece();
	jumpedSquare.setPiece(jumpedPiece);
	m_capturedSquare->setPiece(nullptr);
	if (m_info.promoted)
	{
		fromPiece->demote();
	}
}