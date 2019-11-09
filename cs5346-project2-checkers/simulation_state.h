#pragma once

#include "base_state.h"

#include "genetic_algorithm.h"
#include "player.h"
#include "resource_manager.h"

#include <array>
#include <queue>
#include <utility>

class SimulationState : public BaseState
{
public:
	SimulationState(ResourceManager* pResources);
	~SimulationState() override;
	void enter() override;
	BaseState* event() override;
	void render() override;

private:
	ResourceManager* m_pResources;
	std::array<Player*, 2> m_players;
	checkerboard::Checkerboard m_board;
	std::vector<GeneticEntity> m_population;
	std::vector<GeneticEntity> m_bestFromPreviousGeneration;
	std::default_random_engine m_generator;
	int m_currentGeneration;
	int m_totalFitness;

	bool m_isGameOver;
	std::queue<std::pair<GeneticEntity&, GeneticEntity&>> m_gamesToPlay;

	void resetBoard();

	void initializePopulation(std::size_t populationSize, const GeneticEntity& baseline);
	void setupCompetition();
	void startGame(const std::pair<GeneticEntity&, GeneticEntity&>& players);
	void nextGeneration();

	GeneticEntity reproduce();
	GeneticEntity selectParent();
	GeneticEntity createOffspring(const GeneticEntity& parent1, const GeneticEntity& parent2, float mutationChance);
	GeneticEntity crossover(const GeneticEntity& parent1, const GeneticEntity& parent2);
	GeneticEntity mutate(const GeneticEntity& entity);
};