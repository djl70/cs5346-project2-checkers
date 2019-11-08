#include "genetic_algorithm.h"

Gene::Gene()
	: Gene{ -1.0f, 1.0f, 0.0f }
{

}

Gene::Gene(float min, float max)
	: Gene{ min, max, 0.0f }
{

}

Gene::Gene(float min, float max, float value)
	: m_min{ min }
	, m_max{ max }
	, m_value{ value }
{
	assert(min <= max);
	assert(value >= min);
	assert(value <= max);
}

void Gene::setConstraints(float min, float max)
{
	assert(min <= max);

	m_min = min;
	m_max = max;

	m_value = std::min(m_min, m_value);
	m_value = std::max(m_max, m_value);
}

void Gene::setValue(float value)
{
	assert(value >= m_min);
	assert(value <= m_max);

	m_value = value;
}

float Gene::getMinConstraint() const
{
	return m_min;
}

float Gene::getMaxConstraint() const
{
	return m_max;
}

float Gene::getValue() const
{
	return m_value;
}

Gene randomGene(std::default_random_engine& rng, const Gene& baseline)
{
	std::uniform_real_distribution<float> distribution{ baseline.getMinConstraint(), baseline.getMaxConstraint() };
	return Gene{ baseline.getMinConstraint(), baseline.getMaxConstraint(), distribution(rng) };
}

GeneticEntity::GeneticEntity()
	: m_fitness{ 0 }
{

}

GeneticEntity::GeneticEntity(const std::vector<Gene>& genome)
	: m_genome{ genome }
	, m_fitness{ 0 }
{

}

void GeneticEntity::setGenome(const std::vector<Gene>& genome)
{
	m_genome = genome;
}

void GeneticEntity::addGene(const Gene& gene)
{
	m_genome.push_back(gene);
}

void GeneticEntity::insertGene(std::size_t index, const Gene& gene)
{
	assert(index <= m_genome.size());

	m_genome.insert(m_genome.begin() + index, gene);
}

void GeneticEntity::setGene(std::size_t index, const Gene& gene)
{
	assert(index < m_genome.size());

	m_genome.at(index) = gene;
}

void GeneticEntity::removeGene(std::size_t index)
{
	assert(index < m_genome.size());

	m_genome.erase(m_genome.begin() + index);
}

void GeneticEntity::improveFitness(int amount)
{
	m_fitness += amount;
}

void GeneticEntity::resetFitness()
{
	m_fitness = 0;
}

std::size_t GeneticEntity::countGenes() const
{
	return m_genome.size();
}

Gene GeneticEntity::getGene(std::size_t index) const
{
	assert(index < m_genome.size());

	return m_genome.at(index);
}

std::vector<Gene> GeneticEntity::getGenome() const
{
	return m_genome;
}

int GeneticEntity::getFitness() const
{
	return m_fitness;
}

GeneticEntity randomEntity(std::default_random_engine& rng, const GeneticEntity& baseline)
{
	std::vector<Gene> genome = baseline.getGenome();
	for (auto& gene : genome)
	{
		gene = randomGene(rng, gene);
	}
	return GeneticEntity{ genome };
}