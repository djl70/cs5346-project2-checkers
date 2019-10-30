#pragma once

#include "player.h"

class CheckerSquare;

class HumanPlayer : public Player
{
public:
	HumanPlayer(CheckerColor color);
	void takeTurn() override;
	void event(const sf::Event& event) override;
	Command* update() override;
	void render(sf::RenderWindow* pWindow) override;

private:
	//bool m_isJumpPossible;
	std::vector<CheckerSquare*> m_validMovesFromSelectedSquare;
	CheckerSquare* m_pSelectedSquare;
	bool m_mustJump;
	bool m_checkForAnotherJump;

	Command* m_pCommand;

	std::vector<CheckerSquare*> getValidMovesForSquare(CheckerSquare& from);

	CheckerSquare* getClickedSquare();
	CheckerSquare* findJumpedSquare(CheckerSquare& from, CheckerSquare& to);
};