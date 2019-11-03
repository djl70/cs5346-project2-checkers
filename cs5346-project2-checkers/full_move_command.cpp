#include "full_move_command.h"

#include "checkerboard.h"

FullMoveCommand::FullMoveCommand(checkerboard::Checkerboard& board, const FullMoveInfo& info)
	: m_board{ board }
	, m_info{ info }
	, m_partialExecutionStep{ -1 }
	, m_numTurnsSinceCaptureOrKinging{ board.numTurnsSinceCaptureOrKinging }
{

}

bool FullMoveCommand::executeStep()
{
	// If there are no jumped squares, perform a simple move then return true
	if (m_info.jumped.empty())
	{
		const CheckerSquare& fromSquare = m_board.board.at(checkerboard::index(m_info.from));
		const CheckerSquare& toSquare = m_board.board.at(checkerboard::index(m_info.to.at(0)));

		// CheckerPiece* fromPiece = fromSquare.isEmpty() ? nullptr : &m_board.pieces.at(fromSquare.getPieceIndex());

		if (fromSquare.isEmpty()) { throw ("No piece to move"); }
		if (!toSquare.isEmpty()) { throw ("Cannot move piece to occupied square"); }

		//toSquare.setPieceIndex(fromSquare.getPieceIndex());
		//fromSquare.setPieceIndex(-1);
		movePieceFromTo(m_board, fromSquare.getPieceIndex(), checkerboard::index(m_info.from), checkerboard::index(m_info.to.at(0)));
		return true;
	}

	// Otherwise, perform a single jump (there could be multiple)
	sf::Vector2i fromCoords = m_partialExecutionStep == -1 ? m_info.from : m_info.to.at(m_partialExecutionStep);
	sf::Vector2i jumpedCoords = m_info.jumped.at(m_partialExecutionStep + 1);
	sf::Vector2i toCoords = m_info.to.at(m_partialExecutionStep + 1);

	const CheckerSquare& fromSquare = m_board.board.at(checkerboard::index(fromCoords));
	const CheckerSquare& jumpedSquare = m_board.board.at(checkerboard::index(jumpedCoords));
	const CheckerSquare& toSquare = m_board.board.at(checkerboard::index(toCoords));

	// Move the piece from <from> to <to>
	const CheckerPiece* movedPiece = fromSquare.isEmpty() ? nullptr : &m_board.pieces.at(fromSquare.getPieceIndex());

	if (!movedPiece) { throw ("No piece to move"); }
	if (!toSquare.isEmpty()) { throw ("Cannot move piece to occupied square"); }

	//toSquare.setPieceIndex(fromSquare.getPieceIndex());
	//fromSquare.setPieceIndex(-1);
	movePieceFromTo(m_board, fromSquare.getPieceIndex(), checkerboard::index(fromCoords), checkerboard::index(toCoords));

	// Capture the piece from <jumped>
	const CheckerPiece* jumpedPiece = jumpedSquare.isEmpty() ? nullptr : &m_board.pieces.at(jumpedSquare.getPieceIndex());

	if (!jumpedPiece) { throw ("Cannot jump unoccupied square"); }
	if (jumpedPiece->getColor() == movedPiece->getColor()) { throw ("Cannot jump piece of the same color"); }

	//std::vector<CheckerSquare>& capturedArea = movedPiece->getColor() == kBlack ? m_board.capturedRedSquares : m_board.capturedBlackSquares;

	//// Determine the first available spot in the captured area on the first step
	//if (m_partialExecutionStep == -1)
	//{
	//	for (int i = 0; i < capturedArea.size(); ++i)
	//	{
	//		if (capturedArea.at(i).isEmpty())
	//		{
	//			m_firstCapturedIndex = i;
	//			break;
	//		}
	//	}
	//}

	//CheckerSquare& capturedSquare = capturedArea.at(m_firstCapturedIndex + m_partialExecutionStep + 1);

	//if (!capturedSquare.isEmpty()) { throw ("Cannot place captured piece onto occupied capture area"); }

	int toCapturedIndex = checkerboard::capturePieceFrom(m_board, jumpedSquare.getPieceIndex(), checkerboard::index(jumpedCoords));
	if (m_partialExecutionStep == -1)
	{
		m_firstCapturedIndex = toCapturedIndex;
	}
	//capturedSquare.setPiece(jumpedPiece);
	//jumpedSquare.setPiece(nullptr);

	// Move to the next step
	++m_partialExecutionStep;
	return m_partialExecutionStep == m_info.jumped.size() - 1;
}

bool FullMoveCommand::undoStep()
{
	// If there are no jumped squares, undo the simple move then return true
	if (m_info.jumped.empty())
	{
		const CheckerSquare& fromSquare = m_board.board.at(checkerboard::index(m_info.from));
		const CheckerSquare& toSquare = m_board.board.at(checkerboard::index(m_info.to.at(0)));

		// We get the <from> piece from the <to> square since this is after we have performed the move
		CheckerPiece* fromPiece = toSquare.isEmpty() ? nullptr : &m_board.pieces.at(toSquare.getPieceIndex());

		checkerboard::movePieceFromTo(m_board, toSquare.getPieceIndex(), checkerboard::index(m_info.to.at(0)), checkerboard::index(m_info.from));
		//toSquare.setPiece(nullptr);
		//fromSquare.setPiece(fromPiece);
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

	const CheckerSquare& fromSquare = m_board.board.at(checkerboard::index(fromCoords));
	const CheckerSquare& jumpedSquare = m_board.board.at(checkerboard::index(jumpedCoords));
	const CheckerSquare& toSquare = m_board.board.at(checkerboard::index(toCoords));

	// Move the piece from <to> to <from>
	CheckerPiece* movedPiece = toSquare.isEmpty() ? nullptr : &m_board.pieces.at(toSquare.getPieceIndex());
	checkerboard::movePieceFromTo(m_board, toSquare.getPieceIndex(), checkerboard::index(toCoords), checkerboard::index(fromCoords));
	//toSquare.setPiece(nullptr);
	//fromSquare.setPiece(movedPiece);

	// Return the captured piece to <jumped>
	//std::vector<CheckerSquare>& capturedArea = movedPiece->getColor() == kBlack ? m_board.capturedRedSquares : m_board.capturedBlackSquares;
	//CheckerPiece* jumpedPiece = capturedArea.at(m_firstCapturedIndex + m_partialExecutionStep).getPiece();
	//capturedArea.at(m_firstCapturedIndex + m_partialExecutionStep).setPiece(nullptr);
	//jumpedSquare.setPiece(jumpedPiece);
	checkerboard::releasePieceTo(m_board, movedPiece->getColor() == kBlack ? kRed : kBlack, m_firstCapturedIndex + m_partialExecutionStep, checkerboard::index(jumpedCoords));

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

	// Update board state
	++m_board.turnNumber;
	if (!isJump() && !didPromote())
	{
		++m_board.numTurnsSinceCaptureOrKinging;
	}
	else
	{
		m_board.numTurnsSinceCaptureOrKinging = 0;
	}
	m_board.currentPlayer = checkerboard::nextPlayer(m_board.currentPlayer);

	++m_board.boardStateFrequency[checkerboard::encode(m_board)];
}

void FullMoveCommand::undo()
{
	--m_board.boardStateFrequency[checkerboard::encode(m_board)];

	// Undo all steps
	while (!undoStep())
	{

	}

	// Update board state
	--m_board.turnNumber;
	m_board.numTurnsSinceCaptureOrKinging = m_numTurnsSinceCaptureOrKinging;
	m_board.currentPlayer = checkerboard::nextPlayer(m_board.currentPlayer);

}

bool FullMoveCommand::isJump() const
{
	return !m_info.jumped.empty();
}

bool FullMoveCommand::didPromote() const
{
	return m_info.promoted;
}

bool equal(const FullMoveInfo& lhs, const FullMoveInfo& rhs)
{
	bool match = lhs.from == rhs.from
		&& lhs.to.size() == rhs.to.size()
		&& lhs.jumped.size() == rhs.jumped.size()
		&& lhs.promoted == rhs.promoted;

	if (match)
	{
		for (int i = 0; i < lhs.to.size(); ++i)
		{
			if (lhs.to.at(i) != rhs.to.at(i))
			{
				match = false;
				break;
			}
		}

		if (match)
		{
			for (int i = 0; i < lhs.jumped.size(); ++i)
			{
				if (lhs.jumped.at(i) != rhs.jumped.at(i))
				{
					match = false;
					break;
				}
			}
		}
	}
	
	return match;
}