#include "minimax_search_algorithm.h"

#include <algorithm>
#include <iostream>
#include <limits>

MinimaxSearchAlgorithm::MinimaxSearchAlgorithm(Heuristic* pHeuristic)
	: SearchAlgorithm{ pHeuristic }
{

}

SearchResult MinimaxSearchAlgorithm::findBestMove(const checkerboard::Checkerboard& initialState, int maxDepth, std::promise<void>* exitPromise)
{
	m_result.nodesExpanded = 0;
	m_result.nodesGenerated = 0;
	m_result.searchTime = std::chrono::milliseconds::zero();

	auto t0 = std::chrono::steady_clock::now();
	this->terminate = exitPromise->get_future();
	m_maxDepth = maxDepth;
	CheckerColor currentPlayer = initialState.currentPlayer == 0 ? kBlack : kRed;

	ResultStructure searchResult = minimaxAB(initialState, 0, currentPlayer, std::numeric_limits<int>::max(), -std::numeric_limits<int>::max());

	auto t1 = std::chrono::steady_clock::now();
	m_result.searchTime = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
	m_result.bestMove = searchResult.path.front();

	return m_result;
}

MinimaxSearchAlgorithm::ResultStructure MinimaxSearchAlgorithm::minimaxAB(const checkerboard::Checkerboard& position, int depth, CheckerColor player, int useThreshold, int passThreshold)
{
	if (this->terminate._Is_ready())
		return ResultStructure{ 0, { } };

	if (deepEnough(position, depth))
	{
		return ResultStructure{ value(position, player), { } };
	}

	std::vector<FullMoveInfo> successors = moveGen(position, player);
	++m_result.nodesExpanded;
	m_result.nodesGenerated += successors.size();
	if (successors.empty())
	{
		return ResultStructure{ value(position, player), { } };
	}

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