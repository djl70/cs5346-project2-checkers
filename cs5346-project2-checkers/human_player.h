#pragma once

#include "player.h"

#include "full_move_command.h"

class CheckerSquare;

class HumanPlayer : public Player
{
public:
	HumanPlayer(CheckerColor color);
	void takeTurn() override;
	void event(const sf::Event& event) override;
	FullMoveCommand* update() override;
	void render(sf::RenderWindow* pWindow) override;

private:
	//bool m_isJumpPossible;
	std::vector<CheckerSquare*> m_validMovesFromSelectedSquare;
	CheckerSquare* m_pSelectedSquare;
	bool m_mustJump;
	bool m_checkForAnotherJump;
	bool m_doneBuildingMove;
	FullMoveInfo m_fullMove;

	Checkerboard m_simulatedBoard;

	std::vector<CheckerSquare*> getValidMovesForSquare(CheckerSquare& from);

	CheckerSquare* getClickedSquare();
	CheckerSquare* findJumpedSquare(CheckerSquare& from, CheckerSquare& to);
};