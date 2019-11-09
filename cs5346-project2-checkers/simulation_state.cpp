#include "simulation_state.h"

#include "simulation_player.h"
#include "minimax_search_algorithm.h"
#include "genetic_heuristic.h"

#include <algorithm>
#include <iostream>

SimulationState::SimulationState(ResourceManager* pResources)
	: m_pResources{ pResources }
	, m_generator{ std::chrono::system_clock::now().time_since_epoch().count() }
	, m_population{ 0 }
{

}

SimulationState::~SimulationState()
{
	for (auto& player : m_players)
	{
		if (player)
		{
			delete player;
			player = nullptr;
		}
	}
}

void SimulationState::enter()
{
	// Create players and set up board

	// Play against best from previous generation, plus Kevin Gregor

	// Genetic algorithm should have a list of ancestors and a list of other, finalized heuristics that will always be played against but never updated

	// Create a virtual mutator class that has a mutate() function, and override it to customize how we mutate

	GeneticEntity entity;
	Gene gene{ -1.0f, 1.0f };
	for (int i = 0; i < 12; ++i)
		entity.addGene(gene);
	initializePopulation(100, entity);
	resetBoard();
	setupCompetition();
	m_isGameOver = true;
}

BaseState* SimulationState::event()
{
	// Add a way to save the progress of a simulation and continue from where we left off
	sf::Event event;
	while (m_pResources->getWindow()->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_pResources->getWindow()->close();
			break;

		//case sf::Event::KeyReleased:
		//	if (event.key.code == sf::Keyboard::Key::Enter && m_isGameOver && m_gamesToPlay.empty())
		//	{
		//		setupCompetition();
		//	}
		//	break;
		}

		//m_players.at(m_board.currentPlayer)->event(event);
	}

	if (m_isGameOver)
	{
		if (m_gamesToPlay.empty())
		{
			nextGeneration();
			setupCompetition();
		}

		startGame(m_gamesToPlay.front());
	}

	FullMoveCommand* pCommand = m_players.at(m_board.currentPlayer)->update();
	if (pCommand)
	{
		pCommand->execute();
		delete pCommand;
		pCommand = nullptr;

		GameOverCondition gameOverCondition;
		if (checkerboard::isGameOver(m_board, gameOverCondition))
		{
			switch (gameOverCondition)
			{
			case kBlackCannotMove:
			case kBlackHasNoPiecesLeft:
				m_gamesToPlay.front().first.improveFitness(0);
				m_gamesToPlay.front().second.improveFitness(2);
				std::cout << "Gen " << m_currentGeneration << ": " << m_gamesToPlay.front().first.getID() << " < " << m_gamesToPlay.front().second.getID() << "\n";
				break;
			case kRedCannotMove:
			case kRedHasNoPiecesLeft:
				m_gamesToPlay.front().first.improveFitness(2);
				m_gamesToPlay.front().second.improveFitness(0);
				std::cout << "Gen " << m_currentGeneration << ": " << m_gamesToPlay.front().first.getID() << " > " << m_gamesToPlay.front().second.getID() << "\n";
				break;
			case kBoardStateRepetitionLimitReached:
			case kTurnLimitReached:
			default:
				m_gamesToPlay.front().first.improveFitness(1);
				m_gamesToPlay.front().second.improveFitness(1);
				std::cout << "Gen " << m_currentGeneration << ": " << m_gamesToPlay.front().first.getID() << " = " << m_gamesToPlay.front().second.getID() << "\n";
				break;
			}

			m_gamesToPlay.pop();
			m_isGameOver = true;
		}
		else
		{
			m_players.at(m_board.currentPlayer)->startTurn();
		}
	}

	return nullptr;
}

void SimulationState::render()
{
	// Visualize the simulated games

	// Which player won with which parameters, how many pieces remained, how many turns passed

	m_pResources->getWindow()->clear();
	m_pResources->getWindow()->display();
}

void SimulationState::resetBoard()
{
	m_board.board.clear();
	m_board.capturedBlackSquares.clear();
	m_board.capturedRedSquares.clear();
	m_board.pieces.clear();
	m_board.boardStateFrequency.clear();
	m_board.currentPlayer = 0;
	m_board.numTurnsSinceCaptureOrKinging = 0;
	m_board.turnNumber = 0;

	// Create pieces
	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			if ((r + c) % 2 == 1)
			{
				if (r < 3)
				{
					CheckerPiece piece;
					piece.setColor(kRed);
					//piece.setTextures(m_pResources->getTexture("red_man"), m_pResources->getTexture("red_king"));
					m_board.pieces.push_back(piece);
				}
				else if (r > 4)
				{
					CheckerPiece piece;
					piece.setColor(kBlack);
					//piece.setTextures(m_pResources->getTexture("black_man"), m_pResources->getTexture("black_king"));
					m_board.pieces.push_back(piece);
				}
			}
		}
	}

	// Create squares
	int p = 0;
	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			if ((r + c) % 2 == 1)
			{
				CheckerSquare square{ kBlackSquare };
				square.setPositionOnBoard({ c, r }, { 0, 0 });
				m_board.board.push_back(square);
				if (r < 3)
				{
					if (r == 0)
					{
						m_board.board.back().setPromotionColor(kBlack);
					}
					checkerboard::placePieceAt(m_board, p++, m_board.board.size() - 1);
				}
				else if (r > 4)
				{
					if (r == 7)
					{
						m_board.board.back().setPromotionColor(kRed);
					}
					checkerboard::placePieceAt(m_board, p++, m_board.board.size() - 1);
				}
			}
			else
			{
				CheckerSquare square{ kRedSquare };
				square.setPositionOnBoard({ c, r }, { 0, 0 });
				m_board.board.push_back(square);
			}
		}
	}

	// Create squares for the captured areas
	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 3; ++c)
		{
			CheckerSquare redSquare{ kCapturedRed };
			redSquare.setPositionOnBoard({ c, r }, { 0, 0 });
			m_board.capturedRedSquares.push_back(redSquare);

			CheckerSquare blackSquare{ kCapturedBlack };
			blackSquare.setPositionOnBoard({ c, r }, { 0 ,0 });
			m_board.capturedBlackSquares.push_back(blackSquare);
		}
	}
}

void SimulationState::initializePopulation(std::size_t populationSize, const GeneticEntity& baseline)
{
	m_population.clear();
	int numToKeep = 5;
	for (std::size_t i = 0; i < populationSize; ++i)
	{
		m_population.push_back(randomEntity(m_generator, baseline));
		if (i < numToKeep)
		{
			m_bestFromPreviousGeneration.push_back(randomEntity(m_generator, baseline));
			m_population.push_back(m_bestFromPreviousGeneration.back());
		}
	}
	m_currentGeneration = 0;
	m_totalFitness = 0;
}

void SimulationState::setupCompetition()
{
	// Have each entity play against the others as both black and red
	for (auto& p1 : m_population)
	{
		for (auto& p2 : m_bestFromPreviousGeneration)
		{
			//if (p1 == p2) { continue; }
			//testFitness(m_population.at(p1), m_population.at(p2));
			m_gamesToPlay.push({ p1, p2 });
			m_gamesToPlay.push({ p2, p1 });
		}
	}
}

void SimulationState::startGame(const std::pair<GeneticEntity&, GeneticEntity&>& players)
{
	for (auto& player : m_players)
	{
		if (player)
		{
			delete player;
			player = nullptr;
		}
	}

	m_players.at(0) = new SimulationPlayer{ kBlack, new MinimaxSearchAlgorithm{ new GeneticHeuristic{ weightsFromGenome(players.first) } }, 4 };
	m_players.at(1) = new SimulationPlayer{ kRed,   new MinimaxSearchAlgorithm{ new GeneticHeuristic{ weightsFromGenome(players.second) } }, 4 };

	resetBoard();
	for (auto& player : m_players)
	{
		//player->setResources(m_pResources);
		player->setBoard(&m_board);
	}

	m_isGameOver = false;
	m_players.at(0)->startTurn();
}

void SimulationState::nextGeneration()
{
	m_totalFitness = 0;
	for (const auto& entity : m_population)
	{
		m_totalFitness += entity.getFitness();
	}

	std::vector<GeneticEntity> children;
	for (std::size_t i = 0; i < m_population.size() - m_bestFromPreviousGeneration.size(); ++i)
	{
		children.push_back(reproduce());
	}

	std::sort(m_population.begin(), m_population.end(), [](const auto& a, const auto& b) { return a.getFitness() > b.getFitness(); });
	std::vector<GeneticEntity> best{ m_population.begin(), m_population.begin() + 5 };
	for (int i = 0; i < best.size(); ++i)
	{
		auto& entity = best.at(i);
		std::cout << "Gen " << m_currentGeneration << ": #" << i + 1 << " (fitness = " << entity.getFitness() << ")\n";
		const auto& genome = entity.getGenome();
		for (int j = 0; j < genome.size(); ++j)
		{
			std::cout << "\tGene " << j + 1 << ": " << genome.at(j).getValue() << "\n";
		}
		entity.resetFitness();
	}
	children.insert(children.end(), best.begin(), best.end());

	m_population = children;
	m_bestFromPreviousGeneration = best;
	m_totalFitness = 0;
	++m_currentGeneration;
}

GeneticEntity SimulationState::reproduce()
{
	const float mutationChance = 0.25f;
	return createOffspring(selectParent(), selectParent(), mutationChance);
}

GeneticEntity SimulationState::selectParent()
{
	std::uniform_int_distribution<int> distribution{ 1, m_totalFitness };
	int remaining = distribution(m_generator);
	auto it = m_population.cbegin();
	for (auto it = m_population.cbegin(); it != m_population.cend(); ++it)
	{
		remaining -= it->getFitness();
		if (remaining <= 0)
		{
			return *it;
		}
	}

	throw ("This should never be reached");
}

GeneticEntity SimulationState::createOffspring(const GeneticEntity& parent1, const GeneticEntity& parent2, float mutationChance)
{
	GeneticEntity child = crossover(parent1, parent2);

	std::uniform_real_distribution<float> distribution{ 0.0f, 1.0f };
	if (distribution(m_generator) <= mutationChance)
	{
		child = mutate(child);
	}

	return child;
}

GeneticEntity SimulationState::crossover(const GeneticEntity& parent1, const GeneticEntity& parent2)
{
	GeneticEntity child{ parent2 };
	child.resetFitness();
	child.newID();

	std::uniform_int_distribution<std::size_t> distribution{ 0, parent1.countGenes() };
	std::size_t numGenesFromParent1 = distribution(m_generator);

	auto genome = parent1.getGenome();
	auto it = genome.cbegin();
	for (std::size_t i = 0; i < numGenesFromParent1; ++i)
	{
		child.setGene(i, *(it++));
	}

	return child;
}

GeneticEntity SimulationState::mutate(const GeneticEntity& entity)
{
	std::cout << "Mutation\n";
	std::uniform_int_distribution<std::size_t> distribution{ 0, entity.countGenes() - 1 };
	std::size_t geneIndex = distribution(m_generator);

	GeneticEntity mutated{ entity };
	mutated.setGene(geneIndex, randomGene(m_generator, mutated.getGene(geneIndex)));
	return mutated;
}