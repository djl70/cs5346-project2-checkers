#include "jump_command.h"

#include "checkerboard.h"

JumpInfo::JumpInfo(const checkerboard::Checkerboard& board, const sf::Vector2i& from, const sf::Vector2i& to, const sf::Vector2i& jumped)
	: from{ from }
	, to{ to }
	, jumped{ jumped }
{
	const CheckerSquare& fromSquare = board.board.at(checkerboard::index(from));
	const CheckerPiece* fromPiece = fromSquare.isEmpty() ? nullptr : &board.pieces.at(fromSquare.getPieceIndex());
	const CheckerSquare& toSquare = board.board.at(checkerboard::index(to));

	promoted = fromPiece && !fromPiece->isKing() && toSquare.promotesColor(fromPiece->getColor());
}


JumpCommand::JumpCommand(checkerboard::Checkerboard& board, const JumpInfo& info)
	: m_board{ board }
	, m_info{ info }
	, m_capturedSquare{ -1 }
	, m_numTurnsSinceCaptureOrKinging{ board.numTurnsSinceCaptureOrKinging }
{

}

void JumpCommand::execute()
{
	const CheckerSquare& fromSquare = m_board.board.at(checkerboard::index(m_info.from));
	const CheckerSquare& jumpedSquare = m_board.board.at(checkerboard::index(m_info.jumped));
	const CheckerSquare& toSquare = m_board.board.at(checkerboard::index(m_info.to));

	const CheckerPiece* fromPiece = fromSquare.isEmpty() ? nullptr : &m_board.pieces.at(fromSquare.getPieceIndex());
	if (!fromPiece) { throw ("Error: No piece to jump with"); }
	if (!toSquare.isEmpty()) { throw ("Error: Jumped piece onto occupied square"); }

	checkerboard::movePieceFromTo(m_board, fromSquare.getPieceIndex(), checkerboard::index(m_info.from), checkerboard::index(m_info.to));

	const CheckerPiece* jumpedPiece = jumpedSquare.isEmpty() ? nullptr : &m_board.pieces.at(jumpedSquare.getPieceIndex());
	if (!jumpedPiece) { throw ("Error: No piece to jump over"); }
	if (jumpedPiece->getColor() == fromPiece->getColor()) { throw ("Error: Cannot jump over same-colored piece"); }

	m_capturedSquare = checkerboard::capturePieceFrom(m_board, jumpedSquare.getPieceIndex(), checkerboard::index(m_info.jumped));
	if (m_capturedSquare == -1) { throw ("Error: No more space in the captured area"); }

	// Update board state
	++m_board.turnNumber;
	m_board.numTurnsSinceCaptureOrKinging = 0;
	m_board.currentPlayer = checkerboard::nextPlayer(m_board.currentPlayer);

	++m_board.boardStateFrequency[checkerboard::encode(m_board)];
}

void JumpCommand::undo()
{
	--m_board.boardStateFrequency[checkerboard::encode(m_board)];

	const CheckerSquare& fromSquare = m_board.board.at(checkerboard::index(m_info.from));
	const CheckerSquare& jumpedSquare = m_board.board.at(checkerboard::index(m_info.jumped));
	const CheckerSquare& toSquare = m_board.board.at(checkerboard::index(m_info.to));

	// We will assume that <from> had a piece, <to> did not, and <jumped> had its piece moved to <m_capturedSquare>
	CheckerPiece* fromPiece = toSquare.isEmpty() ? nullptr : &m_board.pieces.at(toSquare.getPieceIndex());
	checkerboard::movePieceFromTo(m_board, toSquare.getPieceIndex(), checkerboard::index(m_info.to), checkerboard::index(m_info.from));
	checkerboard::releasePieceTo(m_board, fromPiece->getColor() == kBlack ? kRed : kBlack, m_capturedSquare, checkerboard::index(m_info.jumped));
	if (m_info.promoted)
	{
		fromPiece->demote();
	}

	// Update board state
	--m_board.turnNumber;
	m_board.numTurnsSinceCaptureOrKinging = m_numTurnsSinceCaptureOrKinging;
	m_board.currentPlayer = checkerboard::nextPlayer(m_board.currentPlayer);
}