#include "human_player.h"

#include "checker_square.h"
#include "checkerboard.h"
#include "resource_manager.h"

HumanPlayer::HumanPlayer(CheckerColor color)
	: Player(color)
	, m_pCommand{ nullptr }
	, m_mustJump{ false }
{

}

void HumanPlayer::takeTurn()
{
	// Determine what we can do: a single move, or as many jumps as possible
	m_mustJump = !findAllValidJumps(*m_pBoard, m_color).empty();
	m_checkForAnotherJump = false;

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
		// If no jumps are possible, we can only perform one move and then our turn is over
		// TODO: If any jumps are possible, we must choose a piece to jump with and jump with that same piece until we can't anymore

		CheckerSquare* clickedSquare = getClickedSquare();
		if (clickedSquare)
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
							// Figure out which square is being jumped
							CheckerSquare* jumpedSquare = findJumpedSquare(*m_pSelectedSquare, *square);
							m_pCommand = new JumpCommand(*m_pBoard, { *m_pSelectedSquare, *square, *jumpedSquare });
						}
						else
						{
							m_pCommand = new MoveCommand(*m_pBoard, { *m_pSelectedSquare, *square });
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

Command* HumanPlayer::update()
{
	Command* pCommand = nullptr;
	
	if (m_checkForAnotherJump)
	{
		m_checkForAnotherJump = false;

		if (findAllValidJumps(*m_pBoard, m_color).empty())
		{
			m_isTurn = false;
			return nullptr;
		}
	}
	
	if (m_pCommand)
	{
		pCommand = m_pCommand;
		m_pCommand = nullptr;

		if (!m_mustJump)
		{
			// We moved, not jumped, so we are done
			m_isTurn = false;
		}
		else
		{
			// We jumped, so only end our turn if there are no other jumps remaining.
			// We'll have to wait for the next loop to check this, though, because our move has not yet been made.
			m_checkForAnotherJump = true;
		}
	}

	return pCommand;
}

void HumanPlayer::render(sf::RenderWindow* pWindow)
{
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
		std::vector<JumpInfo> jumps = findValidJumps(*m_pBoard, m_color, from.getPositionOnBoard());
		for (auto& info : jumps)
		{
			destinations.push_back(&info.to);
		}
	}
	else
	{
		std::vector<MoveInfo> moves = findValidMoves(*m_pBoard, m_color, from.getPositionOnBoard());
		for (auto& info : moves)
		{
			destinations.push_back(&info.to);
		}
	}

	return destinations;
}

CheckerSquare* HumanPlayer::getClickedSquare()
{
	for (int i = 0; i < m_pBoard->board.size(); ++i)
	{
		if (m_pBoard->board[i].contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_pResources->getWindow()))))
		{
			return &m_pBoard->board[i];
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
	return &m_pBoard->board.at(index);
}