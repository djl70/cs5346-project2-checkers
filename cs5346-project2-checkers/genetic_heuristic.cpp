#include "genetic_heuristic.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <random>

GeneticHeuristicWeights randomWeights(bool enableAging)
{
	GeneticHeuristicWeights w;
	std::default_random_engine generator{ std::chrono::system_clock::now().time_since_epoch().count() };

	std::uniform_int_distribution<int> intDistribution{ -100, 100 };
	auto rng = std::bind(intDistribution, generator);
	w.weightPieces = rng();
	w.weightKings = rng();
	w.weightBackRow = rng();
	w.weightMiddleBox = rng();
	w.weightOnALine = rng();
	w.weightBehindALine = rng();
	w.weightBeyondALine = rng();
	w.weightOnDLine = rng();
	w.weightBehindDLine = rng();
	w.weightBeyondDLine = rng();
	w.weightProtected = rng();
	w.weightVulnerable = rng();

	std::uniform_real_distribution<float> realDistribution{ 0.96, 1.04 };
	auto rngAge = std::bind(realDistribution, generator);
	w.maxAge = enableAging ? 100 : 0;
	w.agePieces = enableAging ? rngAge() : 1.0f;
	w.ageKings = enableAging ? rngAge() : 1.0f;
	w.ageBackRow = enableAging ? rngAge() : 1.0f;
	w.ageMiddleBox = enableAging ? rngAge() : 1.0f;
	w.ageOnALine = enableAging ? rngAge() : 1.0f;
	w.ageBehindALine = enableAging ? rngAge() : 1.0f;
	w.ageBeyondALine = enableAging ? rngAge() : 1.0f;
	w.ageOnDLine = enableAging ? rngAge() : 1.0f;
	w.ageBehindDLine = enableAging ? rngAge() : 1.0f;
	w.ageBeyondDLine = enableAging ? rngAge() : 1.0f;
	w.ageProtected = enableAging ? rngAge() : 1.0f;
	w.ageVulnerable = enableAging ? rngAge() : 1.0f;

	return w;
}

GeneticHeuristic::GeneticHeuristic(const GeneticHeuristicWeights& weights)
	: m_kWeights{ weights }
{

}

int GeneticHeuristic::value(const checkerboard::Checkerboard& board) const
{
	CheckerColor playerColor = board.currentPlayer == 0 ? kBlack : kRed;

	// Evaluate an end-game state
	GameOverCondition gameOverCondition;
	if (checkerboard::isGameOver(board, gameOverCondition))
	{
		return terminal(gameOverCondition, playerColor);
	}

	// Values are calculated for both players and the difference is returned

	const int exponent = std::max(board.turnNumber, m_kWeights.maxAge);
	const int weightPieces      = std::powf(m_kWeights.agePieces,      exponent) * m_kWeights.weightPieces;
	const int weightKings       = std::powf(m_kWeights.ageKings,       exponent) * m_kWeights.weightKings;
	const int weightBackRow     = std::powf(m_kWeights.ageBackRow,     exponent) * m_kWeights.weightBackRow;
	const int weightMiddleBox   = std::powf(m_kWeights.ageMiddleBox,   exponent) * m_kWeights.weightMiddleBox;
	const int weightOnALine     = std::powf(m_kWeights.ageOnALine,     exponent) * m_kWeights.weightOnALine;
	const int weightBehindALine = std::powf(m_kWeights.ageBehindALine, exponent) * m_kWeights.weightBehindALine;
	const int weightBeyondALine = std::powf(m_kWeights.ageBeyondALine, exponent) * m_kWeights.weightBeyondALine;
	const int weightOnDLine     = std::powf(m_kWeights.ageOnDLine,     exponent) * m_kWeights.weightOnDLine;
	const int weightBehindDLine = std::powf(m_kWeights.ageBehindDLine, exponent) * m_kWeights.weightBehindDLine;
	const int weightBeyondDLine = std::powf(m_kWeights.ageBeyondDLine, exponent) * m_kWeights.weightBeyondDLine;
	const int weightProtected   = std::powf(m_kWeights.ageProtected,   exponent) * m_kWeights.weightProtected;
	const int weightVulnerable  = std::powf(m_kWeights.ageVulnerable,  exponent) * m_kWeights.weightVulnerable;

	int myValue = 0;
	int otherValue = 0;

	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			if ((r + c) % 2 == 0) { continue; }

			const CheckerSquare& square = board.board.at(checkerboard::index({ c, r }));
			if (square.isEmpty()) { continue; }

			const CheckerPiece& piece = board.pieces.at(square.getPieceIndex());
			CheckerColor squareOwnerColor = piece.getColor();
			int* pValue = squareOwnerColor == playerColor ? &myValue : &otherValue;

			// Value piece
			*pValue += weightPieces;

			// Value king
			if (piece.isKing())
				*pValue += weightKings;

			// Value back row
			if ((r == 0 && squareOwnerColor == kRed) || (r == 7 && squareOwnerColor == kBlack))
				*pValue += weightBackRow;

			// Value middle box
			if ((r == 3 || r == 4) && (c >= 2 && c <= 5))
				*pValue += weightMiddleBox;

			// Value A-diagonal
			if ((squareOwnerColor == kBlack && ((r == 0 && c == 1) || r == c + 1))
				|| (squareOwnerColor == kRed && ((r == 7 && c == 6) || c == r + 1)))
			{
				*pValue += weightOnALine;
			}
			else if ((squareOwnerColor == kBlack && r > c + 1)
				|| (squareOwnerColor == kRed && c > r + 1))
			{
				*pValue += weightBehindALine;
			}
			else
			{
				*pValue += weightBeyondALine;
			}

			// Value D-diagonal
			if (r == 7 - c)
			{
				*pValue += weightOnDLine;
			}
			else if ((squareOwnerColor == kBlack && r > 7 - c)
				|| (squareOwnerColor == kRed && r < 7 - c))
			{
				*pValue += weightBehindDLine;
			}
			else
			{
				*pValue += weightBeyondDLine;
			}

			// Value vulnerability
			*pValue += checkVulnerable(board, squareOwnerColor, { c, r }) ? weightVulnerable : weightProtected;
		}
	}

	return myValue - otherValue;
}

int GeneticHeuristic::terminal(GameOverCondition condition, CheckerColor playerColor) const
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

bool GeneticHeuristic::checkVulnerable(const checkerboard::Checkerboard& board, CheckerColor pieceColor, const sf::Vector2i& start) const
{
	bool isVulnerable = false;

	// Check northwest and southeast neighbors
	const sf::Vector2i coordsNW = { start.x - 1, start.y - 1 };
	const sf::Vector2i coordsSE = { start.x + 1, start.y + 1 };
	if (coordsNW.x >= 0 && coordsNW.x < 8 && coordsNW.y >= 0 && coordsNW.y < 8
		&& coordsSE.x >= 0 && coordsSE.x < 8 && coordsSE.y >= 0 && coordsSE.y < 8)
	{
		const CheckerSquare& neighborNW = board.board.at(checkerboard::index(coordsNW));
		const CheckerSquare& neighborSE = board.board.at(checkerboard::index(coordsSE));

		if (neighborNW.isEmpty() && !neighborSE.isEmpty())
		{
			// Northwest square is empty but southeast is not, so check if southeast is the enemy's piece
			const CheckerPiece& neighborPiece = board.pieces.at(neighborSE.getPieceIndex());
			if (neighborPiece.getColor() != pieceColor)
			{
				isVulnerable = true;
			}
		}
		else if (!neighborNW.isEmpty() && neighborSE.isEmpty())
		{
			// Southeast square is empty but northwest is not, so check if northwest is the enemy's piece
			const CheckerPiece& neighborPiece = board.pieces.at(neighborNW.getPieceIndex());
			if (neighborPiece.getColor() != pieceColor)
			{
				isVulnerable = true;
			}
		}
		// Otherwise, either both NW and SE are empty, or both are occupied; in either case, we are not vulnerable to them
	}

	// Check northeast and southwest neighbors
	const sf::Vector2i coordsNE = { start.x + 1, start.y - 1 };
	const sf::Vector2i coordsSW = { start.x - 1, start.y + 1 };
	if (!isVulnerable
		&& coordsNE.x >= 0 && coordsNE.x < 8 && coordsNE.y >= 0 && coordsNE.y < 8
		&& coordsSW.x >= 0 && coordsSW.x < 8 && coordsSW.y >= 0 && coordsSW.y < 8)
	{
		const CheckerSquare& neighborNE = board.board.at(checkerboard::index(coordsNE));
		const CheckerSquare& neighborSW = board.board.at(checkerboard::index(coordsSW));

		if (neighborNE.isEmpty() && !neighborSW.isEmpty())
		{
			// Northwest square is empty but southeast is not, so check if southeast is the enemy's piece
			const CheckerPiece& neighborPiece = board.pieces.at(neighborSW.getPieceIndex());
			if (neighborPiece.getColor() != pieceColor)
			{
				isVulnerable = true;
			}
		}
		else if (!neighborNE.isEmpty() && neighborSW.isEmpty())
		{
			// Southeast square is empty but northwest is not, so check if northwest is the enemy's piece
			const CheckerPiece& neighborPiece = board.pieces.at(neighborNE.getPieceIndex());
			if (neighborPiece.getColor() != pieceColor)
			{
				isVulnerable = true;
			}
		}
		// Otherwise, either both NE and SW are empty, or both are occupied; in either case, we are not vulnerable to them
	}

	return isVulnerable;
}