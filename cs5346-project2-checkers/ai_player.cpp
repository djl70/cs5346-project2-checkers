#include "ai_player.h"

#include "checkerboard.h"
#include "full_move_command.h"
#include "resource_manager.h"

AIPlayer::AIPlayer(CheckerColor color, SearchAlgorithm* pAlgorithm)
	: Player{ color, true }
	, m_pAlgorithm{ pAlgorithm }
	, m_doneStepping{ false }
	, m_stepDelay{ sf::milliseconds(500) }
	, m_stepCount{ 0 }
{

}

AIPlayer::~AIPlayer()
{
	if (m_pAlgorithm)
	{
		delete m_pAlgorithm;
		m_pAlgorithm = nullptr;
	}
}

void AIPlayer::takeTurn()
{
	// Determine what we can do: a single move, or as many jumps as possible for a single piece
	//m_mustJump = !findAllValidJumps(*m_pBoard, m_color).empty();
	//m_checkForAnotherJump = false;

	// Reset the move
	//m_fullMove.from = nullptr;
	//m_fullMove.jumped.clear();
	//m_fullMove.to.clear();
	//m_fullMove.promoted = false;

	// Clear the previous move if necessary
	if (m_pCommand)
	{
		delete m_pCommand;
		m_pCommand = nullptr;
	}

	// Update the simulated board
	m_simulatedBoard = *m_pBoard;

	// Determine which move to take
	// TODO: Allow the best move to be chosen with a custom function, and run this in a separate thread since it will likely be time consuming and we don't want it to block input
	std::vector<FullMoveInfo> possibleMoves = checkerboard::findAllValidFullMoves(m_simulatedBoard, m_color);
	if (!possibleMoves.empty())
	{
		//m_commandInfo = possibleMoves.at(0);
		m_commandInfo = m_pAlgorithm->findBestMove(m_simulatedBoard, m_color, 4);
		m_pCommand = new FullMoveCommand{ m_simulatedBoard, m_commandInfo };
		m_doneStepping = false;
		m_pFromSquare = &m_simulatedBoard.board.at(checkerboard::index(m_commandInfo.from));
		m_pToSquare = &m_simulatedBoard.board.at(checkerboard::index(m_commandInfo.to.at(0)));
	}
	else
	{
		m_doneStepping = true;
		m_pFromSquare = nullptr;
		m_pToSquare = nullptr;
	}

	m_turnClock.restart();
	m_moveStepTime = sf::Time::Zero;
	m_stepCount = 0;

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

	if (m_doneStepping)
	{
		delete m_pCommand;
		m_pCommand = nullptr;
		pCommand = new FullMoveCommand{ *m_pBoard, m_commandInfo };
		m_isTurn = false;
	}
	else
	{
		m_moveStepTime += m_turnClock.restart();
		while (m_moveStepTime > m_stepDelay)
		{
			m_moveStepTime -= m_stepDelay;
			m_doneStepping = m_pCommand->executeStep();
			++m_stepCount;
			if (!m_doneStepping)
			{
				m_pFromSquare = &m_simulatedBoard.board.at(checkerboard::index(m_commandInfo.to.at(m_stepCount - 1)));
				m_pToSquare = &m_simulatedBoard.board.at(checkerboard::index(m_commandInfo.to.at(m_stepCount)));
			}
		}
	}

	return pCommand;
}

void AIPlayer::render(sf::RenderWindow* pWindow)
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
	if (m_pFromSquare)
	{
		m_pFromSquare->renderHighlight(pWindow);
	}
	if (m_pToSquare)
	{
		m_pToSquare->renderHighlight(pWindow);
	}
}

//JumpInfo AIPlayer::chooseBestJump(const std::vector<JumpInfo>& jumps)
//{
//	return jumps[0];
//}
//
//MoveInfo AIPlayer::chooseBestMove(const std::vector<MoveInfo>& moves)
//{
//	return moves[0];
//}