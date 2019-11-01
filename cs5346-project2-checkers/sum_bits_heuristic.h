#pragma once

#include "heuristic.h"

class SumBitsHeuristic : public Heuristic
{
public:
	int value(const checkerboard::Checkerboard& board, int currentPlayer) const override;
};