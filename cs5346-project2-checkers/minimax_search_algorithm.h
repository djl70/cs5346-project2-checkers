#pragma once

#include "search_algorithm.h"
#include "checker_piece.h"

#include <deque>

class MinimaxSearchAlgorithm : public SearchAlgorithm
{
public:
	MinimaxSearchAlgorithm(Heuristic* pHeuristic);

	SearchResult findBestMove(const checkerboard::Checkerboard& initialState, int maxDepth, std::promise<void>* exitPromise) override;

private:
	struct ResultStructure
	{
		int value;
		std::deque<FullMoveInfo> path;
	};

	int m_maxDepth;
	SearchResult m_result;

	ResultStructure minimaxAB(const checkerboard::Checkerboard& position, int depth, CheckerColor player, int useThreshold, int passThreshold);

	bool deepEnough(const checkerboard::Checkerboard& position, int depth) const;

	int value(const checkerboard::Checkerboard& position, CheckerColor player) const;

	std::vector<FullMoveInfo> moveGen(const checkerboard::Checkerboard& position, CheckerColor player) const;

	CheckerColor opposite(CheckerColor player) const;
};