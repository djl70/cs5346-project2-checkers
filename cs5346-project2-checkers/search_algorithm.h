#pragma once

#include <chrono>
#include <future>

#include "heuristic.h"

struct SearchResult
{
	FullMoveInfo bestMove;
	int nodesGenerated;
	int nodesExpanded;
	std::chrono::milliseconds searchTime;
};

class SearchAlgorithm
{
public:
	SearchAlgorithm(Heuristic* pHeuristic);
	~SearchAlgorithm();

	virtual SearchResult findBestMove(const checkerboard::Checkerboard& initialState, int maxDepth, std::promise<void>* exitPromise) = 0;

protected:
	Heuristic* m_pHeuristic;
	std::future<void> terminate;
};