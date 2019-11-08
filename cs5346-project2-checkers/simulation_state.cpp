#include "simulation_state.h"

SimulationState::SimulationState()
{

}

SimulationState::~SimulationState()
{

}

void SimulationState::enter()
{
	// Create players and set up board

	// Play against best from previous generation, plus Kevin Gregor

	// Genetic algorithm should have a list of ancestors and a list of other, finalized heuristics that will always be played against but never updated

	// Create a virtual mutator class that has a mutate() function, and override it to customize how we mutate
}

BaseState* SimulationState::event()
{
	// Add a way to save the progress of a simulation and continue from where we left off
	return nullptr;
}

void SimulationState::render()
{
	// Visualize the simulated games

	// Which player won with which parameters, how many pieces remained, how many turns passed
}