#pragma once

#include "heuristic.h"

class SearchAlgorithm
{
public:
	SearchAlgorithm(Heuristic* pHeuristic);
	~SearchAlgorithm();

	virtual FullMoveInfo findBestMove(const checkerboard::Checkerboard& initialState, CheckerColor currentPlayer, int maxDepth) = 0;

protected:
	Heuristic* m_pHeuristic;
};