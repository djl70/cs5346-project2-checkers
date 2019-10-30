#include "ai_player.h"

// #include "checker_square.h"
#include "checkerboard.h"
// #include "resource_manager.h"

#include "full_move_command.h"

AIPlayer::AIPlayer(CheckerColor color)
	: Player(color)
	, m_mustJump{ false }
{

}

void AIPlayer::takeTurn()
{
	// Determine what we can do: a single move, or as many jumps as possible for a single piece
	m_mustJump = !findAllValidJumps(*m_pBoard, m_color).empty();
	m_checkForAnotherJump = false;

	// Reset the move
	//m_fullMove.from = nullptr;
	m_fullMove.jumped.clear();
	m_fullMove.to.clear();
	m_fullMove.promoted = false;

	// Update the simulated board
	m_simulatedBoard = *m_pBoard;

	// Call the base function to actually start our turn
	Player::takeTurn();
}

void AIPlayer::event(const sf::Event& event)
{
	// Nothing to do here
}

FullMoveCommand* AIPlayer::update()
{
	FullMoveCommand* pCommand = nullptr;

	while (m_isTurn)
	{
		if (m_checkForAnotherJump)
		{
			m_checkForAnotherJump = false;

			if (m_fullMove.promoted || findValidJumps(m_simulatedBoard, m_color, m_fullMove.to.back()).empty())
			{
				m_isTurn = false;
				break;
			}
		}

		if (m_mustJump)
		{
			if (m_fullMove.to.empty())
			{
				// This is our first jump in a possible series of jumps, so look at all possible jumps and pick the best one
				std::vector<JumpInfo> jumps = findAllValidJumps(m_simulatedBoard, m_color);

				// Determine the best jump
				JumpInfo bestJump = chooseBestJump(jumps);

				m_fullMove.from = bestJump.from;
				m_fullMove.jumped.push_back(bestJump.jumped);
				m_fullMove.to.push_back(bestJump.to);
				m_fullMove.promoted = bestJump.promoted;

				m_simulatedBoard = simulateJump(m_simulatedBoard, bestJump);
			}
			else
			{
				// This is a subsequent jump, so only look at possible jumps for the same piece and pick the best one
				std::vector<JumpInfo> jumps = findValidJumps(m_simulatedBoard, m_color, m_fullMove.to.back());

				// Determine the best jump
				JumpInfo bestJump = chooseBestJump(jumps);

				m_fullMove.jumped.push_back(bestJump.jumped);
				m_fullMove.to.push_back(bestJump.to);
				m_fullMove.promoted = bestJump.promoted;

				m_simulatedBoard = simulateJump(m_simulatedBoard, bestJump);
			}

			m_checkForAnotherJump = true;
		}
		else
		{
			std::vector<MoveInfo> moves = findAllValidMoves(m_simulatedBoard, m_color);

			// Determine the best move
			MoveInfo bestMove = chooseBestMove(moves);

			m_fullMove.from = bestMove.from;
			m_fullMove.to.push_back(bestMove.to);
			m_fullMove.promoted = bestMove.promoted;

			m_isTurn = false;
		}
	}

	pCommand = new FullMoveCommand(*m_pBoard, m_fullMove);
	return pCommand;
}

void AIPlayer::render(sf::RenderWindow* pWindow)
{
	// Nothing special to render for now
	// Maybe consider rendering the piece being moved
}

JumpInfo AIPlayer::chooseBestJump(const std::vector<JumpInfo>& jumps)
{
	return jumps[0];
}

MoveInfo AIPlayer::chooseBestMove(const std::vector<MoveInfo>& moves)
{
	return moves[0];
}