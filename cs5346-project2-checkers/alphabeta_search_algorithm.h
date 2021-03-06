#pragma once

#include "search_algorithm.h"
#include "checker_piece.h"

#include <deque>

class AlphabetaSearchAlgorithm : public SearchAlgorithm
{
public:
	AlphabetaSearchAlgorithm(Heuristic* pHeuristic);

	SearchResult findBestMove(const checkerboard::Checkerboard& initialState, int maxDepth, std::promise<void>* exitPromise) override;

private:
	int m_maxDepth;
	int m_bestValue;
	FullMoveInfo m_actionWithBestValue;
	SearchResult m_result;

	FullMoveInfo alphaBetaSearch(const checkerboard::Checkerboard& state);

	int maxValue(const checkerboard::Checkerboard& state, int depth, int alpha, int beta);

	int minValue(const checkerboard::Checkerboard& state, int depth, int alpha, int beta);

	bool cutoffTest(const checkerboard::Checkerboard& state, int depth) const;

	int eval(const checkerboard::Checkerboard& state) const;

	std::vector<FullMoveInfo> actions(const checkerboard::Checkerboard& state) const;

	checkerboard::Checkerboard result(const checkerboard::Checkerboard& state, const FullMoveInfo& action) const;
};