#include "search_algorithm.h"

SearchAlgorithm::SearchAlgorithm(Heuristic* pHeuristic)
	: m_pHeuristic{ pHeuristic }
{

}

SearchAlgorithm::~SearchAlgorithm()
{
	if (m_pHeuristic)
	{
		delete m_pHeuristic;
		m_pHeuristic = nullptr;
	}
}