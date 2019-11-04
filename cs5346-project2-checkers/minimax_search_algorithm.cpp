#include "minimax_search_algorithm.h"

#include <algorithm>
#include <iostream>
#include <limits>

MinimaxSearchAlgorithm::MinimaxSearchAlgorithm(Heuristic* pHeuristic)
	: SearchAlgorithm{ pHeuristic }
{

}

FullMoveInfo MinimaxSearchAlgorithm::findBestMove(const checkerboard::Checkerboard& initialState, int maxDepth)
{
	m_maxDepth = maxDepth;
	CheckerColor currentPlayer = initialState.currentPlayer == 0 ? kBlack : kRed;
	ResultStructure searchResult = minimaxAB(initialState, 0, currentPlayer, std::numeric_limits<int>::max(), -std::numeric_limits<int>::max());
	return searchResult.path.front();
}

MinimaxSearchAlgorithm::ResultStructure MinimaxSearchAlgorithm::minimaxAB(const checkerboard::Checkerboard& position, int depth, CheckerColor player, int useThreshold, int passThreshold) const
{
	if (deepEnough(position, depth))
	{
		return ResultStructure{ value(position, player), { } };
	}

	std::vector<FullMoveInfo> successors = moveGen(position, player);
	if (successors.empty())
	{
		return ResultStructure{ value(position, player), { } };
	}

	// Shuffle possible moves so that we are not making moves that are as deterministic
	//std::random_shuffle(successors.begin(), successors.end());

	std::deque<FullMoveInfo> bestPath;
	for (const FullMoveInfo& succ : successors)
	{
		// Perform the move to create the successor's position
		checkerboard::Checkerboard positionSucc{ position };
		FullMoveCommand moveCommand{ positionSucc, succ };
		moveCommand.execute();

		ResultStructure resultSucc = minimaxAB(positionSucc, depth + 1, opposite(player), -passThreshold, -useThreshold);
		int newValue = -resultSucc.value;
		if (newValue > passThreshold)
		{
			passThreshold = newValue;
			bestPath = resultSucc.path;
			bestPath.push_front(succ);
		}

		if (passThreshold >= useThreshold)
		{
			return ResultStructure{ passThreshold, bestPath };
		}
	}

	return ResultStructure{ passThreshold, bestPath };
}

bool MinimaxSearchAlgorithm::deepEnough(const checkerboard::Checkerboard& position, int depth) const
{
	return depth >= m_maxDepth;
}

int MinimaxSearchAlgorithm::value(const checkerboard::Checkerboard& position, CheckerColor player) const
{
	return m_pHeuristic->value(position);
}

std::vector<FullMoveInfo> MinimaxSearchAlgorithm::moveGen(const checkerboard::Checkerboard& position, CheckerColor player) const
{
	return checkerboard::findAllValidFullMoves(position, player);
}

CheckerColor MinimaxSearchAlgorithm::opposite(CheckerColor player) const
{
	return player == kBlack ? kRed : kBlack;
}