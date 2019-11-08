#pragma once

#include "heuristic.h"
#include "genetic_algorithm.h"

struct GeneticHeuristicWeights
{
	float weightPieces;
	float weightKings;
	float weightBackRow;
	float weightMiddleBox;
	float weightOnALine;
	float weightBehindALine;
	float weightBeyondALine;
	float weightOnDLine;
	float weightBehindDLine;
	float weightBeyondDLine;
	float weightProtected;
	float weightVulnerable;

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

GeneticHeuristicWeights weightsFromGenome(const GeneticEntity& entity);

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