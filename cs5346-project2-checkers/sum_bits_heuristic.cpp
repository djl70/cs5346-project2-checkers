#include "sum_bits_heuristic.h"

int SumBitsHeuristic::value(const checkerboard::Checkerboard& board) const
{
	return checkerboard::encode(board).count();
}