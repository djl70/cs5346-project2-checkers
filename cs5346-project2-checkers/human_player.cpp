#include "human_player.h"

#include "checker_square.h"
#include "checkerboard.h"
#include "resource_manager.h"

HumanPlayer::HumanPlayer(CheckerColor color)
	: Player{ color, false }
	, m_mustJump{ false }
{

}

void HumanPlayer::takeTurn()
{
	// Determine what we can do: a single move, or as many jumps as possible for a single piece
	m_mustJump = !checkerboard::findAllValidJumps(*m_pBoard, m_color).empty();
	m_checkForAnotherJump = false;
	m_doneBuildingMove = false;

	// Reset the move
	//m_fullMove.from = nullptr;
	m_fullMove.jumped.clear();
	m_fullMove.to.clear();
	m_fullMove.promoted = false;

	// Update the simulated board
	m_simulatedBoard = *m_pBoard;

	// Start our turn with no square selected
	m_pSelectedSquare = nullptr;
	m_validMovesFromSelectedSquare.clear();

	// Call the base function to actually start our turn
	Player::takeTurn();
}

void HumanPlayer::event(const sf::Event& event)
{
	// Do nothing while we're waiting to check for more jumps
	if (m_checkForAnotherJump)
	{
		return;
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
	{
		// 1. No moves made yet
		//   - Freely select and move/jump with a piece as it has been
		// 2. One or more jumps made and more jumps possible
		//   - Automatically select the piece that was already moved in the jump and highlight the possible move-to squares

		const CheckerSquare* clickedSquare = getClickedSquare();
		if (!clickedSquare)
		{
			return;
		}

		if (m_mustJump && !m_fullMove.to.empty())
		{
			// Check if the clicked square is one of the valid moves
			for (const CheckerSquare* forcedSquare : m_validMovesFromSelectedSquare)
			{
				if (forcedSquare == clickedSquare)
				{
					// We did click a valid square to jump to, so do it
					const CheckerSquare* jumpedSquare = findJumpedSquare(*m_pSelectedSquare, *forcedSquare);
					JumpInfo info{ *m_pBoard, m_pSelectedSquare->getPositionOnBoard(), forcedSquare->getPositionOnBoard(), jumpedSquare->getPositionOnBoard() };

					m_fullMove.to.push_back(info.to);
					m_fullMove.jumped.push_back(info.jumped);
					m_fullMove.promoted = info.promoted;

					m_simulatedBoard = checkerboard::simulateJump(m_simulatedBoard, info);
					m_checkForAnotherJump = true;

					break;
				}
			}
		}
		else
		{
			const CheckerPiece* piece = clickedSquare->isEmpty() ? nullptr : &m_pBoard->pieces.at(clickedSquare->getPieceIndex());
			if (piece && piece->getColor() == m_color)
			{
				// Perform a new selection
				m_pSelectedSquare = clickedSquare;
				m_validMovesFromSelectedSquare = getValidMovesForSquare(*clickedSquare);
			}
			else if (m_pSelectedSquare && !piece)
			{
				// Try to move/jump
				for (const CheckerSquare* square : m_validMovesFromSelectedSquare)
				{
					if (square == clickedSquare)
					{
						// The square we clicked is one of the possible destinations, so perform the move/jump
						if (m_mustJump)
						{
							const CheckerSquare* jumpedSquare = findJumpedSquare(*m_pSelectedSquare, *square);
							JumpInfo info{ *m_pBoard, m_pSelectedSquare->getPositionOnBoard(), square->getPositionOnBoard(), jumpedSquare->getPositionOnBoard() };

							// This is the first jump
							m_fullMove.from = info.from;
							m_fullMove.to.push_back(info.to);
							m_fullMove.jumped.push_back(info.jumped);
							m_fullMove.promoted = info.promoted;

							m_simulatedBoard = simulateJump(m_simulatedBoard, info);
							m_checkForAnotherJump = true;
						}
						else
						{
							// We are just performing a simple move
							MoveInfo info{ *m_pBoard, m_pSelectedSquare->getPositionOnBoard(), square->getPositionOnBoard() };

							m_fullMove.from = info.from;
							m_fullMove.jumped.clear();
							m_fullMove.to.push_back(info.to);
							m_fullMove.promoted = info.promoted;

							m_doneBuildingMove = true;
						}

						// Clear the selected square now that we've moved
						m_pSelectedSquare = nullptr;
						m_validMovesFromSelectedSquare.clear();

						break;
					}
				}
			}
		}
	}
}

FullMoveCommand* HumanPlayer::update()
{
	FullMoveCommand* pCommand = nullptr;
	
	if (m_checkForAnotherJump)
	{
		m_checkForAnotherJump = false;

		if (m_fullMove.promoted || findValidJumps(m_simulatedBoard, m_color, m_fullMove.to.back()).empty())
		{
			m_doneBuildingMove = true;
		}
		else
		{
			// We must jump, and we have already performed a jump previously
			// In this case, we want to make sure the selected piece is the one that was already moved
			sf::Vector2i forcedIndex = m_fullMove.to.back();
			m_pSelectedSquare = &m_simulatedBoard.board.at(checkerboard::index(forcedIndex));
			m_validMovesFromSelectedSquare = getValidMovesForSquare(*m_pSelectedSquare);
		}
	}

	if (m_doneBuildingMove)
	{
		pCommand = new FullMoveCommand(*m_pBoard, m_fullMove);
		m_isTurn = false;
	}

	return pCommand;
}

void HumanPlayer::render(sf::RenderWindow* pWindow)
{
	// Draw simulated game board so that it updates as the turn progresses
	for (auto& square : m_simulatedBoard.board)
	{
		square.render(pWindow);
	}

	for (auto& square : m_simulatedBoard.capturedRedSquares)
	{
		square.render(pWindow);
	}

	for (auto& square : m_simulatedBoard.capturedBlackSquares)
	{
		square.render(pWindow);
	}

	for (auto& piece : m_simulatedBoard.pieces)
	{
		piece.render(pWindow);
	}

	// Draw highlighted squares last
	if (m_pSelectedSquare)
	{
		for (const CheckerSquare* valid : m_validMovesFromSelectedSquare)
		{
			valid->renderHighlight(pWindow);
		}
		m_pSelectedSquare->renderHighlight(pWindow);
	}
}

std::vector<const CheckerSquare*> HumanPlayer::getValidMovesForSquare(const CheckerSquare& from)
{
	std::vector<const CheckerSquare*> destinations;

	if (m_mustJump)
	{
		std::vector<JumpInfo> jumps = checkerboard::findValidJumps(m_simulatedBoard, m_color, from.getPositionOnBoard());
		for (auto& info : jumps)
		{
			destinations.push_back(&m_simulatedBoard.board.at(checkerboard::index(info.to)));
		}
	}
	else
	{
		std::vector<MoveInfo> moves = checkerboard::findValidMoves(m_simulatedBoard, m_color, from.getPositionOnBoard());
		for (auto& info : moves)
		{
			destinations.push_back(&m_simulatedBoard.board.at(checkerboard::index(info.to)));
		}
	}

	return destinations;
}

const CheckerSquare* HumanPlayer::getClickedSquare()
{
	for (int i = 0; i < m_simulatedBoard.board.size(); ++i)
	{
		if (m_simulatedBoard.board[i].contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_pResources->getWindow()))))
		{
			return &m_simulatedBoard.board[i];
			break;
		}
	}

	return nullptr;
}

const CheckerSquare* HumanPlayer::findJumpedSquare(const CheckerSquare& from, const CheckerSquare& to)
{
	sf::Vector2i toCoord = to.getPositionOnBoard();
	sf::Vector2i fromCoord = from.getPositionOnBoard();
	sf::Vector2i diff = toCoord - fromCoord;
	sf::Vector2i half = diff / 2;
	sf::Vector2i jumpedCoord = fromCoord + half;
	return &m_simulatedBoard.board.at(checkerboard::index(jumpedCoord));
}