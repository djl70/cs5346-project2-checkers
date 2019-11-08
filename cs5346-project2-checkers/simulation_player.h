#pragma once

#include "player.h"

#include "search_algorithm.h"

#include <thread>

class SimulationPlayer : public Player
{
public:
	SimulationPlayer(CheckerColor color, SearchAlgorithm* pAlgorithm);
	~SimulationPlayer() override;

	void startTurn() override;
	void stop() override;
	void event(const sf::Event& event) override;
	FullMoveCommand* update() override;
	void render(sf::RenderWindow* pWindow) override;

private:
	SearchAlgorithm* m_pAlgorithm;
	std::promise<void>* m_pExitSignal;
	FullMoveInfo m_moveInfo;
};