#include "checkerboard.h"
#include "heuristic_DK.h"

std::vector<DKHeuristic::PieceInfo> DKHeuristic::getPiecesInfo(const checkerboard::Checkerboard& board) const {
	std::vector<DKHeuristic::PieceInfo> info;

	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			if ((r + c) % 2 == 1)
			{
				const CheckerSquare& square = board.board.at(checkerboard::index({ c, r }));
				if (square.isEmpty())
				{
					continue;
				}

				const CheckerPiece& piece = board.pieces.at(square.getPieceIndex());
				info.emplace_back(PieceInfo{ r, c, piece });
			}
		}
	}

	return info;
}

bool DKHeuristic::ifPieceOfCurrentPlayer(const checkerboard::Checkerboard& board, const CheckerPiece& piece) const {
	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;
	int squareOwner = piece.getColor() == playerColor ? board.currentPlayer : checkerboard::nextPlayer(board.currentPlayer);
	CheckerColor squareOwnerColor = squareOwner == 0 ? kBlack : kRed;
	return squareOwnerColor == playerColor;
}

int DKHeuristic::value(const checkerboard::Checkerboard& board) const
{
	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	GameOverCondition gameOverCondition;
	if (checkerboard::isGameOver(board, gameOverCondition))
	{
		return terminal(gameOverCondition, playerColor);
	}

	// Some elements of the evaluation function are used from
	// https://www.mini.pw.edu.pl/~mandziuk/PRACE/es_init.pdf by Jacek Mandziuk et al.
	// Values are calculated for both players and the difference is returned

	// The best so far
	/*const int wMen = 10;
	const int wKings = 20;
	const int wSafeMen = 3;
	const int wSafeKings = 1;
	const int wMovableMen = 3;
	const int wMovableKings = 5;
	const double wSumOfDistancesToPromote = -0.1;
	const int wFreeSquaresForPromotion = 2;
	const int wAttackers = 3;
	const int wDefenders = 2;
	const int wCentralMen = -2;
	const int wCentralKings = 2;*/

	const int wMen = 10;
	const int wKings = 20;
	const int wSafeMen = 3;
	const int wSafeKings = 1;
	const int wMenCanCapture = 3;
	const int wKingsCanCapture = 5;
	const double wSumOfDistancesToPromote = -0.1;
	const int wFreeSquaresForPromotion = 2;
	const int wAttackers = 3;
	const int wDefenders = 2;
	const int wCentralMen = -2;
	const int wCentralKings = 2;
	/*const int wMainDiagonalMen = -2;
	const int wMainDiagonalKings = 2;*/
	/*const int wDoubleDiagonalMen = -1;
	const int wDoubleDiagonalKings = 1;*/

	int value = 0;

	value += wMen * countMen(board);
	value += wKings * countKings(board);
	value += wSafeMen * countSafeMen(board);
	value += wSafeKings * countSafeKings(board);
	value += wMenCanCapture * countMenCanCapture(board);
	value += wKingsCanCapture * countKingsCanCapture(board);
	value += wSumOfDistancesToPromote * sumOfDistancesToPromote(board);
	value += wFreeSquaresForPromotion * freeSquaresForPromotion(board);
	value += wAttackers * countAttackers(board);
	value += wDefenders * countDefenders(board);
	value += wCentralMen * countCentral(board);
	value += wCentralKings * countCentral(board, true);
	/*value += wMainDiagonalMen * countMainDiagonal(board);
	value += wMainDiagonalKings * countMainDiagonal(board, true);*/
	/*value += wDoubleDiagonalMen * countDoubleDiagonal(board);
	value += wDoubleDiagonalKings * countDoubleDiagonal(board, true);*/

	return value;
}

int DKHeuristic::terminal(GameOverCondition condition, CheckerColor playerColor) const
{
	switch (condition)
	{
	case kBlackCannotMove:
	case kBlackHasNoPiecesLeft:
		return playerColor == kBlack ? -std::numeric_limits<int>::max() + 1 : std::numeric_limits<int>::max() - 1;
	case kRedCannotMove:
	case kRedHasNoPiecesLeft:
		return playerColor == kRed ? -std::numeric_limits<int>::max() + 1 : std::numeric_limits<int>::max() - 1;
	case kTurnLimitReached:
	case kBoardStateRepetitionLimitReached:
	default:
		return 0;
	}
}

int DKHeuristic::countMen(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		if (!i.piece.isKing())
			++(*value);
	}

	return myValue - otherValue;
}

int DKHeuristic::countKings(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		if (i.piece.isKing())
			++(*value);
	}

	return myValue - otherValue;
}

int DKHeuristic::countSafeMen(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool isMen = !i.piece.isKing();
		bool isSafe = (i.r % 7 == 0 || i.c % 7 == 0);
		if (isMen && isSafe)
			++(*value);
	} 

	return myValue - otherValue;
}

int DKHeuristic::countSafeKings(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool isKing = i.piece.isKing();
		bool isSafe = (i.r % 7 == 0 || i.c % 7 == 0);
		if (isKing && isSafe)
			++(*value);
	}

	return myValue - otherValue;
}

int DKHeuristic::countMenCanCapture(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool isMen = !i.piece.isKing();
		bool captureAvailable = !checkerboard::findValidJumps(board, playerColor, { i.c, i.r }).empty();
		bool moveAvailable = !checkerboard::findValidMoves(board, playerColor, { i.c, i.r }).empty();
		if (isMen && captureAvailable && moveAvailable)
			++(*value);
	}

	return myValue - otherValue;
}

int DKHeuristic::countKingsCanCapture(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool isKing = i.piece.isKing();
		bool captureAvailable = !checkerboard::findValidJumps(board, playerColor, { i.c, i.r }).empty();
		bool moveAvailable = !checkerboard::findValidMoves(board, playerColor, { i.c, i.r }).empty();
		if (isKing && captureAvailable && moveAvailable)
			++(*value);
	}

	return myValue - otherValue;
}

int DKHeuristic::sumOfDistancesToPromote(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool isMen = !i.piece.isKing();
		if (isMen) {
			int distance = (playerColor == kBlack? i.r: 7 - i.r);
			*value += distance;
		}
		
	}

	return myValue - otherValue;
}

int DKHeuristic::freeSquaresForPromotion(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool onPromotionLine = (playerColor == kBlack ? i.r == 7 : i.r == 0);
		if (onPromotionLine) {
			++(*value);
		}

	}

	return myValue - otherValue;
}

int DKHeuristic::countDefenders(const checkerboard::Checkerboard& board) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool onDefendingLines = (playerColor == kBlack ? i.r > 5 : i.r < 2);
		if (onDefendingLines) {
			++(*value);
		}

	}

	return myValue - otherValue;
}

int DKHeuristic::countAttackers(const checkerboard::Checkerboard& board, bool checkForKing) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool correctType = (checkForKing ? i.piece.isKing() : !i.piece.isKing());
		bool onAttackingLines = (playerColor == kBlack ? i.r < 3 : i.r > 4);
		if (correctType && onAttackingLines) {
			++(*value);
		}

	}

	return myValue - otherValue;
}

int DKHeuristic::countCentral(const checkerboard::Checkerboard& board, bool checkForKing) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool correctType = (checkForKing ? i.piece.isKing() : !i.piece.isKing());
		bool onCentralSquare = (i.r > 2 && i.r < 7 && i.c > 2 && i.c < 7);
		if (correctType && onCentralSquare) {
			++(*value);
		}

	}

	return myValue - otherValue;
}

int DKHeuristic::countMainDiagonal(const checkerboard::Checkerboard& board, bool checkForKing) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool correctType = (checkForKing? i.piece.isKing(): !i.piece.isKing());
		bool onMainDiagonal = (abs(i.r - i.c) == 0);
		if (correctType && onMainDiagonal) {
			++(*value);
		}

	}

	return myValue - otherValue;
}

int DKHeuristic::countDoubleDiagonal(const checkerboard::Checkerboard& board, bool checkForKing) const
{
	int myValue = 0;
	int otherValue = 0;

	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	std::vector<PieceInfo> info = getPiecesInfo(board);
	for (PieceInfo &i : info) {
		int *value = ifPieceOfCurrentPlayer(board, i.piece) ? &myValue : &otherValue;
		bool correctType = (checkForKing ? i.piece.isKing() : !i.piece.isKing());
		bool onDoubleDiagonal = (abs(i.r - i.c) == 6);
		if (correctType && onDoubleDiagonal) {
			++(*value);
		}

	}

	return myValue - otherValue;
}

//int DKHeuristic::countVulnerablePieces(const checkerboard::Checkerboard& board) const
//{
//	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;
//	int myValue = 0;
//	int otherValue = 0;
//
//	for (int r = 0; r < 8; ++r)
//	{
//		for (int c = 0; c < 8; ++c)
//		{
//			if ((r + c) % 2 == 1)
//			{
//				const CheckerSquare& square = board.board.at(checkerboard::index({ c, r }));
//				if (square.isEmpty())
//				{
//					continue;
//				}
//
//				const CheckerPiece& piece = board.pieces.at(square.getPieceIndex());
//				int squareOwner = piece.getColor() == playerColor ? board.currentPlayer : checkerboard::nextPlayer(board.currentPlayer);
//				CheckerColor squareOwnerColor = squareOwner == 0 ? kBlack : kRed;
//				int* value = squareOwnerColor == playerColor ? &myValue : &otherValue;
//
//				bool isVulnerable = false;
//
//				// Check northwest and southeast neighbors
//				const sf::Vector2i coordsNW = { c - 1, r - 1 };
//				const sf::Vector2i coordsSE = { c + 1, r + 1 };
//				if (coordsNW.x >= 0 && coordsNW.x < 8 && coordsNW.y >= 0 && coordsNW.y < 8
//					&& coordsSE.x >= 0 && coordsSE.x < 8 && coordsSE.y >= 0 && coordsSE.y < 8)
//				{
//					const CheckerSquare& neighborNW = board.board.at(checkerboard::index(coordsNW));
//					const CheckerSquare& neighborSE = board.board.at(checkerboard::index(coordsSE));
//
//					if (neighborNW.isEmpty() && !neighborSE.isEmpty())
//					{
//						// Northwest square is empty but southeast is not, so check if southeast is the enemy's piece
//						const CheckerPiece& neighborPiece = board.pieces.at(neighborSE.getPieceIndex());
//						if (neighborPiece.getColor() != squareOwnerColor)
//						{
//							isVulnerable = true;
//						}
//					}
//					else if (!neighborNW.isEmpty() && neighborSE.isEmpty())
//					{
//						// Southeast square is empty but northwest is not, so check if northwest is the enemy's piece
//						const CheckerPiece& neighborPiece = board.pieces.at(neighborNW.getPieceIndex());
//						if (neighborPiece.getColor() != squareOwnerColor)
//						{
//							isVulnerable = true;
//						}
//					}
//					// Otherwise, either both NW and SE are empty, or both are occupied; in either case, we are not vulnerable to them
//				}
//
//				// Check northeast and southwest neighbors
//				const sf::Vector2i coordsNE = { c + 1, r - 1 };
//				const sf::Vector2i coordsSW = { c - 1, r + 1 };
//				if (!isVulnerable
//					&& coordsNE.x >= 0 && coordsNE.x < 8 && coordsNE.y >= 0 && coordsNE.y < 8
//					&& coordsSW.x >= 0 && coordsSW.x < 8 && coordsSW.y >= 0 && coordsSW.y < 8)
//				{
//					const CheckerSquare& neighborNE = board.board.at(checkerboard::index(coordsNE));
//					const CheckerSquare& neighborSW = board.board.at(checkerboard::index(coordsSW));
//
//					if (neighborNE.isEmpty() && !neighborSW.isEmpty())
//					{
//						// Northwest square is empty but southeast is not, so check if southeast is the enemy's piece
//						const CheckerPiece& neighborPiece = board.pieces.at(neighborSW.getPieceIndex());
//						if (neighborPiece.getColor() != squareOwnerColor)
//						{
//							isVulnerable = true;
//						}
//					}
//					else if (!neighborNE.isEmpty() && neighborSW.isEmpty())
//					{
//						// Southeast square is empty but northwest is not, so check if northwest is the enemy's piece
//						const CheckerPiece& neighborPiece = board.pieces.at(neighborNE.getPieceIndex());
//						if (neighborPiece.getColor() != squareOwnerColor)
//						{
//							isVulnerable = true;
//						}
//					}
//					// Otherwise, either both NE and SW are empty, or both are occupied; in either case, we are not vulnerable to them
//				}
//
//				if (isVulnerable)
//				{
//					++(*value);
//				}
//			}
//		}
//	}
//
//	return myValue - otherValue;
//}