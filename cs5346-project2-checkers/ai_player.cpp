#include "ai_player.h"

// #include "checker_square.h"
#include "checkerboard.h"
// #include "resource_manager.h"

AIPlayer::AIPlayer(CheckerColor color)
	: Player(color)
	, m_mustJump{ false }
{

}

void AIPlayer::takeTurn()
{
	// Determine what we can do: a single move, or as many jumps as possible
	m_mustJump = !findAllValidJumps(*m_pBoard, m_color).empty();
	m_checkForAnotherJump = false;

	// Call the base function to actually start our turn
	Player::takeTurn();
}

void AIPlayer::event(const sf::Event& event)
{
	// Nothing to do here
}

Command* AIPlayer::update()
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

	if (m_mustJump)
	{
		std::vector<JumpInfo> jumps = findAllValidJumps(*m_pBoard, m_color);

		// Determine the best jump
		JumpInfo bestJump = chooseBestJump(jumps);
		pCommand = new JumpCommand(*m_pBoard, bestJump);

		m_checkForAnotherJump = true;
	}
	else
	{
		std::vector<MoveInfo> moves = findAllValidMoves(*m_pBoard, m_color);

		// Determine the best move
		MoveInfo bestMove = chooseBestMove(moves);
		pCommand = new MoveCommand(*m_pBoard, bestMove);

		m_isTurn = false;
	}

	sf::sleep(sf::milliseconds(500));

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