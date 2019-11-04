#include "alphabeta_search_algorithm.h"

#include <algorithm>
#include <limits>

AlphabetaSearchAlgorithm::AlphabetaSearchAlgorithm(Heuristic* pHeuristic)
	: SearchAlgorithm{ pHeuristic }
{

}

FullMoveInfo AlphabetaSearchAlgorithm::findBestMove(const checkerboard::Checkerboard& initialState, int maxDepth)
{
	m_maxDepth = maxDepth;
	m_bestValue = -std::numeric_limits<int>::max();
	FullMoveInfo searchResult = alphaBetaSearch(initialState);
	return searchResult;
}

FullMoveInfo AlphabetaSearchAlgorithm::alphaBetaSearch(const checkerboard::Checkerboard& state)
{
	int v = maxValue(state, 0, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

	// return the action in ACTIONS(state) with value v
	return m_actionWithBestValue;
}

int AlphabetaSearchAlgorithm::maxValue(const checkerboard::Checkerboard& state, int depth, int alpha, int beta)
{
	if (cutoffTest(state, depth))
	{
		return eval(state);
	}

	int v = -std::numeric_limits<int>::max();
	std::vector<FullMoveInfo> moves = actions(state);
	for (const auto& a : moves)
	{
		int vNew = minValue(result(state, a), depth + 1, alpha, beta);
		v = std::max(v, vNew);

		// TODO: I think the way we handle m_actionWithBestValue can potentially lead to repeated moves, which are invalid and will cause an exception
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
	if (cutoffTest(state, depth))
	{
		return eval(state);
	}

	int v = std::numeric_limits<int>::max();
	std::vector<FullMoveInfo> moves = actions(state);
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