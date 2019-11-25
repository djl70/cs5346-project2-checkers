#pragma once

#include "player.h"

#include "checkerboard.h"
#include "full_move_command.h"
#include "search_algorithm.h"

#include <SFML/System.hpp>

#include <thread>

class FullMoveCommand;

class AIPlayer : public Player
{
public:
	AIPlayer(CheckerColor color, SearchAlgorithm* pAlgorithm);
	~AIPlayer();

	void startTurn() override;
	void stop() override;
	void event(const sf::Event& event) override;
	FullMoveCommand* update() override;
	void render(sf::RenderWindow* pWindow) override;

private:
	SearchAlgorithm* m_pAlgorithm;
	std::promise<void>* exitSignal;
	std::thread m_moveSelectionThread;
	sf::Clock m_turnClock;
	sf::Time m_moveStepTime;
	bool m_doneBuildingMove;
	bool m_doneStepping;
	int m_stepCount;

	int m_totalNodesGenerated;
	int m_totalNodesExpanded;
	std::chrono::milliseconds m_totalSearchTime;

	const CheckerSquare* m_pFromSquare;
	const CheckerSquare* m_pToSquare;

	FullMoveInfo m_commandInfo;
	FullMoveCommand* m_pCommand;
	checkerboard::Checkerboard m_simulatedBoard;

	const sf::Time m_stepDelay;

	void selectMove();
};