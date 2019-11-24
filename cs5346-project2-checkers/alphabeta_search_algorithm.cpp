#include "alphabeta_search_algorithm.h"

#include <algorithm>
#include <limits>

AlphabetaSearchAlgorithm::AlphabetaSearchAlgorithm(Heuristic* pHeuristic)
	: SearchAlgorithm{ pHeuristic }
{

}

SearchResult AlphabetaSearchAlgorithm::findBestMove(const checkerboard::Checkerboard& initialState, int maxDepth, std::promise<void>* exitPromise)
{
	m_result.nodesExpanded = 0;
	m_result.nodesGenerated = 0;
	m_result.searchTime = std::chrono::milliseconds::zero();

	auto t0 = std::chrono::steady_clock::now();
	this->terminate = exitPromise->get_future();
	m_maxDepth = maxDepth;
	m_bestValue = -std::numeric_limits<int>::max();
	
	FullMoveInfo searchResult = alphaBetaSearch(initialState);
	
	auto t1 = std::chrono::steady_clock::now();
	m_result.searchTime = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
	m_result.bestMove = searchResult;
	
	return m_result;
}

FullMoveInfo AlphabetaSearchAlgorithm::alphaBetaSearch(const checkerboard::Checkerboard& state)
{
	int v = maxValue(state, 0, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

	// return the action in ACTIONS(state) with value v
	return m_actionWithBestValue;
}

int AlphabetaSearchAlgorithm::maxValue(const checkerboard::Checkerboard& state, int depth, int alpha, int beta)
{
	if (this->terminate._Is_ready())
		return 0;

	if (cutoffTest(state, depth))
	{
		return eval(state);
	}

	int v = -std::numeric_limits<int>::max();
	
	std::vector<FullMoveInfo> moves = actions(state);
	++m_result.nodesExpanded;
	m_result.nodesGenerated += moves.size();

	for (const auto& a : moves)
	{
		int vNew = minValue(result(state, a), depth + 1, alpha, beta);
		v = std::max(v, vNew);

		// Keep track of the action that leads to the best value
		if (depth == 0 && vNew > m_bestValue)
		{
			m_bestValue = v;
			m_actionWithBestValue = a;
		}

		if (v >= beta)
		{
			return v;
		}

		alpha = std::max(alpha, v);
	}
	
	return v;
}

int AlphabetaSearchAlgorithm::minValue(const checkerboard::Checkerboard& state, int depth, int alpha, int beta)
{
	if (this->terminate._Is_ready())
		return 0;

	if (cutoffTest(state, depth))
	{
		return eval(state);
	}

	int v = std::numeric_limits<int>::max();

	std::vector<FullMoveInfo> moves = actions(state);
	++m_result.nodesExpanded;
	m_result.nodesGenerated += moves.size();

	for (const auto& a : moves)
	{
		v = std::min(v, maxValue(result(state, a), depth + 1, alpha, beta));

		if (v <= alpha)
		{
			return v;
		}

		beta = std::min(beta, v);
	}

	return v;
}

bool AlphabetaSearchAlgorithm::cutoffTest(const checkerboard::Checkerboard& state, int depth) const
{
	GameOverCondition gameOverCondition; // not used, but needed for isGameOver()
	return depth >= m_maxDepth || checkerboard::isGameOver(state, gameOverCondition);
}

int AlphabetaSearchAlgorithm::eval(const checkerboard::Checkerboard& state) const
{
	return m_pHeuristic->value(state);
}

std::vector<FullMoveInfo> AlphabetaSearchAlgorithm::actions(const checkerboard::Checkerboard& state) const
{
	return checkerboard::findAllValidFullMoves(state, state.currentPlayer == 0 ? kBlack : kRed);
}

checkerboard::Checkerboard AlphabetaSearchAlgorithm::result(const checkerboard::Checkerboard& state, const FullMoveInfo& action) const
{
	checkerboard::Checkerboard board{ state };
	FullMoveCommand command{ board, action };
	command.execute();
	return board;
}