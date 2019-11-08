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

void SimulationPlayer::startTurn()
{
	// Search for a move, stopping after 100 ms
	if (m_pCommand)
	{
		delete m_pCommand;
		m_pCommand = nullptr;
	}
}

void SimulationPlayer::stop()
{
	// Join the move selection thread here
	m_pExitSignal->set_value();
}

void SimulationPlayer::event(const sf::Event& event)
{

}

FullMoveCommand* SimulationPlayer::update()
{

}

void SimulationPlayer::render(sf::RenderWindow* pWindow)
{

}