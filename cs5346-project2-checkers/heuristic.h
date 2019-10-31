#pragma once

#include "checkerboard.h"

class Heuristic
{
public:
	// Determine the value or "goodness" of a given board state
	virtual int value(const checkerboard::Checkerboard& board) const = 0;
};