#include "move_command.h"

#include "checkerboard.h"

MoveInfo::MoveInfo(const checkerboard::Checkerboard& board, const sf::Vector2i& from, const sf::Vector2i& to)
	: from{ from }
	, to{ to }
{
	const CheckerSquare& fromSquare = board.board.at(checkerboard::index(from));
	const CheckerPiece* fromPiece = fromSquare.isEmpty() ? nullptr : &board.pieces.at(fromSquare.getPieceIndex());
	const CheckerSquare& toSquare = board.board.at(checkerboard::index(to));

	promoted = fromPiece && !fromPiece->isKing() && toSquare.promotesColor(fromPiece->getColor());
}

MoveCommand::MoveCommand(checkerboard::Checkerboard& board, const MoveInfo& info)
	: m_board{ board }
	, m_info{ info }
{

}

void MoveCommand::execute()
{
	const CheckerSquare& fromSquare = m_board.board.at(checkerboard::index(m_info.from));
	const CheckerSquare& toSquare = m_board.board.at(checkerboard::index(m_info.to));

	const CheckerPiece* fromPiece = fromSquare.isEmpty() ? nullptr : &m_board.pieces.at(fromSquare.getPieceIndex());
	if (!fromPiece) { throw ("Error: No piece to move with"); }
	if (!toSquare.isEmpty()) { throw ("Error: Moved piece onto occupied square"); }

	checkerboard::movePieceFromTo(m_board, fromSquare.getPieceIndex(), checkerboard::index(m_info.from), checkerboard::index(m_info.to));
}

void MoveCommand::undo()
{
	const CheckerSquare& fromSquare = m_board.board.at(checkerboard::index(m_info.from));
	const CheckerSquare& toSquare = m_board.board.at(checkerboard::index(m_info.to));

	CheckerPiece* fromPiece = fromSquare.isEmpty() ? nullptr : &m_board.pieces.at(toSquare.getPieceIndex());
	checkerboard::movePieceFromTo(m_board, toSquare.getPieceIndex(), checkerboard::index(m_info.to), checkerboard::index(m_info.from));

	if (m_info.promoted)
	{
		fromPiece->demote();
	}
}