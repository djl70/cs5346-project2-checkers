#pragma once

#include "heuristic.h"

class Heuristic_1 : public Heuristic
{
public:
	int value(const checkerboard::Checkerboard& board) const override;

private:
	int earlyGame(const checkerboard::Checkerboard& board) const;
	int lateGame(const checkerboard::Checkerboard& board) const;
	int terminal(GameOverCondition condition, CheckerColor playerColor) const;
	bool isTrapped(const checkerboard::Checkerboard& board, const sf::Vector2i& squareCoord) const;
};