#pragma once

#include "player.h"

#include "jump_command.h"
#include "move_command.h"
#include "full_move_command.h"

class AIPlayer : public Player
{
public:
	AIPlayer(CheckerColor color);
	// AIPlayer(CheckerColor color, EvaluationFunction* eval);
	void takeTurn() override;
	void event(const sf::Event& event) override;
	Command* update() override;
	void render(sf::RenderWindow* pWindow) override;

private:
	bool m_mustJump;
	bool m_checkForAnotherJump;
	bool m_doneBuildingMove;
	FullMoveInfo m_fullMove;

	CheckerSquare* m_pJumpedTo;

	JumpInfo chooseBestJump(const std::vector<JumpInfo>& jumps);
	MoveInfo chooseBestMove(const std::vector<MoveInfo>& moves);
};