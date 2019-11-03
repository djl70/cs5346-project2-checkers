#pragma once

#include "heuristic.h"

class Heuristic_1 : public Heuristic
{
public:
	int value(const checkerboard::Checkerboard& board) const override;
};