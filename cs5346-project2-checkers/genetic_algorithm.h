#pragma once

#include "genetic_heuristic.h"
#include <vector>

class GeneticMutator
{
public:
	virtual GeneticHeuristicWeights mutate(const GeneticHeuristicWeights& source) const = 0;
};

class GeneticAlgorithm
{
public:
	std::vector<GeneticHeuristicWeights> initialize(int populationSize);
	// TODO: Selection (evaluate fitness of each element of the population and build a mating pool)
	// TODO: Reproduction (pick 2 parents with probability according to relative fitness, create a child by combining DNA, mutate child's DNA based on a probability, and add child to new population)
	// TODO: Replace old population with new population of mutated children

private:

};