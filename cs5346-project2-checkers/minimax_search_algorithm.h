#pragma once

#include "search_algorithm.h"
#include "checker_piece.h"

#include <deque>

class MinimaxSearchAlgorithm : public SearchAlgorithm
{
public:
	MinimaxSearchAlgorithm(Heuristic* pHeuristic);

	FullMoveInfo findBestMove(const checkerboard::Checkerboard& initialState, CheckerColor currentPlayer, int maxDepth) override;

private:
	struct ResultStructure
	{
		int value;
		std::deque<FullMoveInfo> path;
	};

	int m_maxDepth;

	ResultStructure minimaxAB(const checkerboard::Checkerboard& position, int depth, CheckerColor player, int useThreshold, int passThreshold) const;

	bool deepEnough(const checkerboard::Checkerboard& position, int depth) const;

	int value(const checkerboard::Checkerboard& position, CheckerColor player) const;

	std::vector<FullMoveInfo> moveGen(const checkerboard::Checkerboard& position, CheckerColor player) const;

	CheckerColor opposite(CheckerColor player) const;
};