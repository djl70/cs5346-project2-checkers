#include "simulation_player.h"

SimulationPlayer::SimulationPlayer(CheckerColor color, SearchAlgorithm* pAlgorithm)
	: Player{ color, true }
	, m_pAlgorithm{ pAlgorithm }
	, m_pExitSignal{ nullptr }
{

}

SimulationPlayer::~SimulationPlayer()
{
	stop();

	if (m_pAlgorithm)
	{
		delete m_pAlgorithm;
		m_pAlgorithm = nullptr;
	}
}

void SimulationPlayer::startTurn()
{
	// Search for a move, stopping after 100 ms
	if (m_pExitSignal)
	{
		delete m_pExitSignal;
		m_pExitSignal = nullptr;
	}
	m_pExitSignal = new std::promise<void>;

	m_moveInfo = m_pAlgorithm->findBestMove(*m_pBoard, 2, m_pExitSignal);
}

void SimulationPlayer::stop()
{
	m_pExitSignal->set_value();
}

void SimulationPlayer::event(const sf::Event& event)
{
	// Do nothing, won't be called
}

FullMoveCommand* SimulationPlayer::update()
{
	// Since we determine our move in startTurn(), just return that move here
	return new FullMoveCommand{ *m_pBoard, m_moveInfo };
}

void SimulationPlayer::render(sf::RenderWindow* pWindow)
{
	// Do nothing, won't be called
}