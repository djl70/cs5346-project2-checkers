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
	m_mustJump = !findAllValidJumps(*m_pBoard, m_color).empty();
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

		CheckerSquare* clickedSquare = getClickedSquare();
		if (!clickedSquare)
		{
			return;
		}

		if (m_mustJump && !m_fullMove.to.empty())
		{
			// Check if the clicked square is one of the valid moves
			for (CheckerSquare* forcedSquare : m_validMovesFromSelectedSquare)
			{
				if (forcedSquare == clickedSquare)
				{
					// We did click a valid square to jump to, so do it
					CheckerSquare* jumpedSquare = findJumpedSquare(*m_pSelectedSquare, *forcedSquare);
					JumpInfo info{ *m_pSelectedSquare, *forcedSquare, *jumpedSquare };

					m_fullMove.to.push_back(info.to);
					m_fullMove.jumped.push_back(info.jumped);
					m_fullMove.promoted = info.promoted;

					m_simulatedBoard = simulateJump(m_simulatedBoard, info);
					m_checkForAnotherJump = true;

					break;
				}
			}
		}
		else
		{
			CheckerPiece* piece = clickedSquare->getPiece();
			if (piece && piece->getColor() == m_color)
			{
				// Perform a new selection
				m_pSelectedSquare = clickedSquare;
				m_validMovesFromSelectedSquare = getValidMovesForSquare(*clickedSquare);
			}
			else if (m_pSelectedSquare && !piece)
			{
				// Try to move/jump
				for (CheckerSquare* square : m_validMovesFromSelectedSquare)
				{
					if (square == clickedSquare)
					{
						// The square we clicked is one of the possible destinations, so perform the move/jump
						if (m_mustJump)
						{
							CheckerSquare* jumpedSquare = findJumpedSquare(*m_pSelectedSquare, *square);
							JumpInfo info{ *m_pSelectedSquare, *square, *jumpedSquare };

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
							MoveInfo info{ *m_pSelectedSquare, *square };

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
			m_pSelectedSquare = &m_simulatedBoard.board.at(forcedIndex.y * 8 + forcedIndex.x);
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
	// TODO: Fix issue where a square is sometimes rendered on top of the piece during a multi-jump

	// Draw highlighted squares
	if (m_pSelectedSquare)
	{
		for (CheckerSquare* valid : m_validMovesFromSelectedSquare)
		{
			valid->renderHighlight(m_pResources->getWindow());
		}
		m_pSelectedSquare->renderHighlight(m_pResources->getWindow());
	}
}

std::vector<CheckerSquare*> HumanPlayer::getValidMovesForSquare(CheckerSquare& from)
{
	std::vector<CheckerSquare*> destinations;

	if (m_mustJump)
	{
		std::vector<JumpInfo> jumps = findValidJumps(m_simulatedBoard, m_color, from.getPositionOnBoard());
		for (auto& info : jumps)
		{
			destinations.push_back(&m_simulatedBoard.board.at(info.to.y * 8 + info.to.x));
		}
	}
	else
	{
		std::vector<MoveInfo> moves = findValidMoves(m_simulatedBoard, m_color, from.getPositionOnBoard());
		for (auto& info : moves)
		{
			destinations.push_back(&m_simulatedBoard.board.at(info.to.y * 8 + info.to.x));
		}
	}

	return destinations;
}

CheckerSquare* HumanPlayer::getClickedSquare()
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

CheckerSquare* HumanPlayer::findJumpedSquare(CheckerSquare& from, CheckerSquare& to)
{
	sf::Vector2i toCoord = to.getPositionOnBoard();
	sf::Vector2i fromCoord = from.getPositionOnBoard();
	sf::Vector2i diff = toCoord - fromCoord;
	sf::Vector2i half = diff / 2;
	sf::Vector2i jumpedCoord = fromCoord + half;
	int index = jumpedCoord.y * 8 + jumpedCoord.x;
	return &m_simulatedBoard.board.at(index);
}