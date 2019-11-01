#include "sum_bits_heuristic.h"

int SumBitsHeuristic::value(const checkerboard::Checkerboard& board, int currentPlayer) const
{
	return checkerboard::encode(board, currentPlayer).count();
}