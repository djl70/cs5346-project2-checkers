#pragma once

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <random>
#include <vector>

class Gene
{
public:
	Gene();
	Gene(float min, float max);
	Gene(float min, float max, float value);

	void setConstraints(float min, float max);
	void setValue(float value);
	
	float getMinConstraint() const;
	float getMaxConstraint() const;
	float getValue() const;

private:
	float m_min;
	float m_max;
	float m_value;
};

Gene randomGene(std::default_random_engine& rng, const Gene& baseline);



class GeneticEntity
{
public:
	GeneticEntity();
	GeneticEntity(const std::vector<Gene>& genome);

	void setGenome(const std::vector<Gene>& genome);
	void addGene(const Gene& gene);
	void insertGene(std::size_t index, const Gene& gene);
	void setGene(std::size_t index, const Gene& gene);
	void removeGene(std::size_t index);
	void improveFitness(int amount);
	void resetFitness();
	void newID();

	std::size_t countGenes() const;
	Gene getGene(std::size_t index) const;
	std::vector<Gene> getGenome() const;
	int getFitness() const;
	int getID() const;

private:
	std::vector<Gene> m_genome;
	int m_fitness;
	int m_id;

	static int nextID;
};

GeneticEntity randomEntity(std::default_random_engine& rng, const GeneticEntity& baseline);