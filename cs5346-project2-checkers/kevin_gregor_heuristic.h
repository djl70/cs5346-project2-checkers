#pragma once

#include "heuristic.h"

class KevinGregorHeuristic : public Heuristic
{
public:
	int value(const checkerboard::Checkerboard& board) const override;

private:
	int terminal(GameOverCondition condition, CheckerColor playerColor) const;
	int countMen(const checkerboard::Checkerboard& board) const;
	int countKings(const checkerboard::Checkerboard& board) const;
	int countBackRow(const checkerboard::Checkerboard& board) const;
	int countMiddleBox(const checkerboard::Checkerboard& board) const;
	int countMiddleRows(const checkerboard::Checkerboard& board) const;
	int countVulnerablePieces(const checkerboard::Checkerboard& board) const;
	int countProtectedPieces(const checkerboard::Checkerboard& board) const;
};