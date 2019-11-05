#pragma once

#include "heuristic.h"

class Heuristic_2 : public Heuristic
{
public:
	int value(const checkerboard::Checkerboard& board) const override;

private:
	int terminal(GameOverCondition condition, CheckerColor playerColor) const;
};