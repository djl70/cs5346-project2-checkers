#include "jump_command.h"

#include "checkerboard.h"

JumpInfo::JumpInfo(CheckerSquare& from, CheckerSquare& to, CheckerSquare& jumped)
	: from{ from }
	, to{ to }
	, jumped{ jumped }
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
	CheckerPiece* fromPiece = m_info.from.getPiece();
	if (fromPiece)
	{
		CheckerPiece* toPiece = m_info.to.getPiece();
		if (toPiece)
		{
			throw ("Error: Jumped piece onto occupied square");
		}
		m_info.to.setPiece(fromPiece);
		m_info.from.setPiece(nullptr);

		CheckerPiece* jumpedPiece = m_info.jumped.getPiece();
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
				m_info.jumped.setPiece(nullptr);
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
	// We will assume that <from> had a piece, <to> did not, and <jumped> had its piece moved to <m_capturedSquare>
	CheckerPiece* fromPiece = m_info.to.getPiece();
	m_info.from.setPiece(fromPiece);
	m_info.to.setPiece(nullptr);
	CheckerPiece* jumpedPiece = m_capturedSquare->getPiece();
	m_info.jumped.setPiece(jumpedPiece);
	m_capturedSquare->setPiece(nullptr);
	if (m_info.promoted)
	{
		fromPiece->demote();
	}
}