#include <iostream>
#include <future>

#include "ai_player.h"

#include "checkerboard.h"
#include "full_move_command.h"
#include "resource_manager.h"

#include "config.h"

AIPlayer::AIPlayer(CheckerColor color, SearchAlgorithm* pAlgorithm)
	: Player{ color, true }
	, m_pAlgorithm{ pAlgorithm }
	, m_doneBuildingMove{ false }
	, m_doneStepping{ false }
	, m_stepDelay{ sf::milliseconds(500) }
	, m_stepCount{ 0 }
	, exitSignal{ nullptr }
{

}

AIPlayer::~AIPlayer()
{
	stop();

	if (m_pCommand)
	{
		delete m_pCommand;
		m_pCommand = nullptr;
	}

	if (m_pAlgorithm)
	{
		delete m_pAlgorithm;
		m_pAlgorithm = nullptr;
	}
}

void AIPlayer::startTurn()
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

	m_doneBuildingMove = false;

	// Update the simulated board
	m_simulatedBoard = *m_pBoard;
	m_pFromSquare = nullptr;
	m_pToSquare = nullptr;

	// Simulate thread termination through the use of futures
	delete exitSignal;
	exitSignal = new std::promise<void>;

	// Select moves in a separate thread so it doesn't block input
	if (m_moveSelectionThread.joinable())
	{
		m_moveSelectionThread.join();
	}
	m_moveSelectionThread = std::thread{ &AIPlayer::selectMove, this };

	// Call the base function to actually start our turn
	//Player::startTurn();
}

void AIPlayer::stop()
{
	try {
		exitSignal->set_value();
	}
	catch (std::exception ex) { }
	// Join the move selection thread here
	if (m_moveSelectionThread.joinable())
	{
		m_moveSelectionThread.join();
	}

}

void AIPlayer::event(const sf::Event& event)
{
	// Nothing to do here
}

FullMoveCommand* AIPlayer::update()
{
	FullMoveCommand* pCommand = nullptr;

	// Wait until we have chosen which move to take 
	if (!m_doneBuildingMove)
	{
		return nullptr;
	}

	// Wait until we have finished animating our move
	if (!m_doneStepping)
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
	else
	{
		delete m_pCommand;
		m_pCommand = nullptr;
		pCommand = new FullMoveCommand{ *m_pBoard, m_commandInfo };
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

void AIPlayer::selectMove()
{

	std::vector<FullMoveInfo> possibleMoves = checkerboard::findAllValidFullMoves(m_simulatedBoard, m_color);
	if (!possibleMoves.empty())
	{
		m_commandInfo = m_pAlgorithm->findBestMove(m_simulatedBoard, config::kMaxSearchDepth, this->exitSignal);
		m_pCommand = new FullMoveCommand{ m_simulatedBoard, m_commandInfo };
		m_doneStepping = false;
		m_pFromSquare = &m_simulatedBoard.board.at(checkerboard::index(m_commandInfo.from));
		m_pToSquare = &m_simulatedBoard.board.at(checkerboard::index(m_commandInfo.to.at(0)));
	}
	else
	{
		m_doneStepping = true;
	}

	m_turnClock.restart();
	m_moveStepTime = sf::Time::Zero;
	m_stepCount = 0;

	m_doneBuildingMove = true;
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