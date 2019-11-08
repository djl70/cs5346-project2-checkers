#pragma once

#include "heuristic.h"

struct GeneticHeuristicWeights
{
	int weightPieces;
	int weightKings;
	int weightBackRow;
	int weightMiddleBox;
	int weightOnALine;
	int weightBehindALine;
	int weightBeyondALine;
	int weightOnDLine;
	int weightBehindDLine;
	int weightBeyondDLine;
	int weightProtected;
	int weightVulnerable;

	// Ages should probably be within the range [0.95, 1.05] to prevent insane scaling
	int maxAge;
	float agePieces;
	float ageKings;
	float ageBackRow;
	float ageMiddleBox;
	float ageOnALine;
	float ageBehindALine;
	float ageBeyondALine;
	float ageOnDLine;
	float ageBehindDLine;
	float ageBeyondDLine;
	float ageProtected;
	float ageVulnerable;
};

GeneticHeuristicWeights randomWeights(bool enableAging);

class GeneticHeuristic : public Heuristic
{
public:
	GeneticHeuristic(const GeneticHeuristicWeights& weights);

	int value(const checkerboard::Checkerboard& board) const override;

private:
	const GeneticHeuristicWeights m_kWeights;

	int terminal(GameOverCondition condition, CheckerColor playerColor) const;
	bool checkVulnerable(const checkerboard::Checkerboard& board, CheckerColor pieceColor, const sf::Vector2i& start) const;
};