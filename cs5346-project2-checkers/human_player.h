#pragma once

#include "player.h"

#include "checkerboard.h"
#include "full_move_command.h"

class CheckerSquare;

class HumanPlayer : public Player
{
public:
	HumanPlayer(CheckerColor color);
	~HumanPlayer() {};

	void startTurn() override;
	void stop() override;
	void event(const sf::Event& event) override;
	FullMoveCommand* update() override;
	void render(sf::RenderWindow* pWindow) override;

private:
	//bool m_isJumpPossible;
	std::vector<const CheckerSquare*> m_validMovesFromSelectedSquare;
	const CheckerSquare* m_pSelectedSquare;
	bool m_mustJump;
	bool m_checkForAnotherJump;
	bool m_doneBuildingMove;
	FullMoveInfo m_fullMove;

	checkerboard::Checkerboard m_simulatedBoard;

	std::vector<const CheckerSquare*> getValidMovesForSquare(const CheckerSquare& from);

	const CheckerSquare* getClickedSquare();
	const CheckerSquare* findJumpedSquare(const CheckerSquare& from, const CheckerSquare& to);
};