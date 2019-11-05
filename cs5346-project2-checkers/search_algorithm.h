#pragma once

#include <future>

#include "heuristic.h"

class SearchAlgorithm
{
public:
	SearchAlgorithm(Heuristic* pHeuristic);
	~SearchAlgorithm();

	virtual FullMoveInfo findBestMove(const checkerboard::Checkerboard& initialState, int maxDepth, std::promise<void>* exitPromise) = 0;

protected:
	Heuristic* m_pHeuristic;
	std::future<void> terminate;
};