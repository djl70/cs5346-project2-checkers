#include "move_command.h"

#include "checkerboard.h"

MoveInfo::MoveInfo(CheckerSquare& from, CheckerSquare& to)
	: from{ from }
	, to{ to }
{

}

MoveCommand::MoveCommand(Checkerboard& board, const MoveInfo& info)
	: m_board{ board }
	, m_info{ info }
{

}

void MoveCommand::execute()
{
	CheckerPiece* fromPiece = m_info.from.getPiece();
	if (fromPiece)
	{
		CheckerPiece* toPiece = m_info.to.getPiece();
		if (toPiece)
		{
			throw ("Error: Moved piece onto occupied square");
		}
		m_info.to.setPiece(fromPiece);
		m_info.from.setPiece(nullptr);
	}
	else
	{
		throw ("Error: No piece to move");
	}
}

void MoveCommand::undo()
{
	// We will assume that <from> had a piece and that <to> did not
	CheckerPiece* fromPiece = m_info.to.getPiece();
	m_info.from.setPiece(fromPiece);
	m_info.to.setPiece(nullptr);
}