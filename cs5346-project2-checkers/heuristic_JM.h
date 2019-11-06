#pragma once

#include "heuristic.h"

class JMHeuristic : public Heuristic
{
public:
	int value(const checkerboard::Checkerboard& board) const override;

private:
	struct PieceInfo {
		int r, c;
		const CheckerPiece& piece;

		PieceInfo(int row, int col, const CheckerPiece &checkerPiece) :
			r(row), c(col), piece(checkerPiece) {}
	};

	std::vector<PieceInfo> getPiecesInfo(const checkerboard::Checkerboard& board) const;
	bool ifPieceOfCurrentPlayer(const checkerboard::Checkerboard& board, const CheckerPiece& piece) const;

	int terminal(GameOverCondition condition, CheckerColor playerColor) const;
	int countMen(const checkerboard::Checkerboard& board) const;
	int countKings(const checkerboard::Checkerboard& board) const;
	int countSafeMen(const checkerboard::Checkerboard& board) const;
	int countSafeKings(const checkerboard::Checkerboard& board) const;
	int countMoveableMen(const checkerboard::Checkerboard& board) const;
	int countMoveableKings(const checkerboard::Checkerboard& board) const;
	int sumOfDistancesToPromote(const checkerboard::Checkerboard& board) const;
	int freeSquaresForPromotion(const checkerboard::Checkerboard& board) const;

	int countDefenders(const checkerboard::Checkerboard& board) const;
	int countAttackers(const checkerboard::Checkerboard& board, bool checkForKing = false) const;
	int countCentral(const checkerboard::Checkerboard& board, bool checkForKing = false) const;
	int countMainDiagonal(const checkerboard::Checkerboard& board, bool checkForKing = false) const;
	int countDoubleDiagonal(const checkerboard::Checkerboard& board, bool checkForKing = false) const;
	// int countVulnerablePieces(const checkerboard::Checkerboard& board) const;
};