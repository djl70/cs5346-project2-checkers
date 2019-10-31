#pragma once

#include "player.h"

//#include "jump_command.h"
//#include "move_command.h"
#include "full_move_command.h"

#include "checkerboard.h"

#include <SFML/System.hpp>

class FullMoveCommand;

class AIPlayer : public Player
{
public:
	AIPlayer(CheckerColor color);
	// AIPlayer(CheckerColor color, EvaluationFunction* eval);
	void takeTurn() override;
	void event(const sf::Event& event) override;
	FullMoveCommand* update() override;
	void render(sf::RenderWindow* pWindow) override;

private:
	sf::Clock m_turnClock;
	sf::Time m_moveStepTime;
	bool m_doneStepping;
	int m_stepCount;

	const CheckerSquare* m_pFromSquare;
	const CheckerSquare* m_pToSquare;

	FullMoveInfo m_commandInfo;
	FullMoveCommand* m_pCommand;
	checkerboard::Checkerboard m_simulatedBoard;

	const sf::Time m_stepDelay;

	//bool m_mustJump;
	//bool m_checkForAnotherJump;
	//FullMoveInfo m_fullMove;

	//Checkerboard m_simulatedBoard;

	//JumpInfo chooseBestJump(const std::vector<JumpInfo>& jumps);
	//MoveInfo chooseBestMove(const std::vector<MoveInfo>& moves);
};