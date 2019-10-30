#include "move_command.h"

#include "checkerboard.h"

MoveInfo::MoveInfo(CheckerSquare& from, CheckerSquare& to)
	: from{ from.getPositionOnBoard() }
	, to{ to.getPositionOnBoard() }
	, promoted{ from.getPiece() && !from.getPiece()->isKing() && to.promotesColor(from.getPiece()->getColor()) }
{

}

MoveCommand::MoveCommand(Checkerboard& board, const MoveInfo& info)
	: m_board{ board }
	, m_info{ info }
{

}

void MoveCommand::execute()
{
	CheckerSquare& fromSquare = m_board.board.at(m_info.from.y * 8 + m_info.from.x);
	CheckerSquare& toSquare = m_board.board.at(m_info.to.y * 8 + m_info.to.x);

	CheckerPiece* fromPiece = fromSquare.getPiece();
	if (fromPiece)
	{
		CheckerPiece* toPiece = toSquare.getPiece();
		if (toPiece)
		{
			throw ("Error: Moved piece onto occupied square");
		}
		toSquare.setPiece(fromPiece);
		fromSquare.setPiece(nullptr);
	}
	else
	{
		throw ("Error: No piece to move with");
	}
}

void MoveCommand::undo()
{
	CheckerSquare& fromSquare = m_board.board.at(m_info.from.y * 8 + m_info.from.x);
	CheckerSquare& toSquare = m_board.board.at(m_info.to.y * 8 + m_info.to.x);

	// We will assume that <from> had a piece and that <to> did not
	CheckerPiece* fromPiece = toSquare.getPiece();
	fromSquare.setPiece(fromPiece);
	toSquare.setPiece(nullptr);

	if (m_info.promoted)
	{
		fromPiece->demote();
	}
}