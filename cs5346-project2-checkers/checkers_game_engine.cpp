#include "checkers_game_engine.h"

#include "main_menu.h"

#include "human_player.h"

#include "jump_command.h"
#include "move_command.h"

#include "renderer.h"
#include "resources.h"

CheckersGameEngine::CheckersGameEngine(sf::RenderWindow* pWindow)
	: m_pWindow{ pWindow }
	, m_boardRect{ 560.0f, 140.0f, 800.0f, 800.0f } // assuming 1920x1080 resolution
	, m_gui{ nullptr }
{

}

CheckersGameEngine::~CheckersGameEngine()
{
	for (Player* player : m_gameState.players)
	{
		delete player;
	}
}

void CheckersGameEngine::performCommand(Command* command)
{
	command->execute();
	m_commandHistory.push(command);
}

void CheckersGameEngine::undoLastCommand()
{
	Command* command = m_commandHistory.top();
	command->undo();

	m_commandHistory.pop();
	delete command;
}

void CheckersGameEngine::run()
{
	if (!resources::loadAllResources())
	{
		return;
	}
	applyTextures();
	initializeGameState();

	m_gui = new MainMenu;
	mainLoop();
}

void CheckersGameEngine::mainLoop()
{
	while (m_pWindow->isOpen())
	{
		sf::Event event;
		while (m_pWindow->pollEvent(event))
		{
			processEvent(event);
		}

		Command* playerCommand = m_gameState.players.at(m_gameState.currentPlayer)->takeTurn(m_gameState);
		if (playerCommand)
		{
			playerCommand->execute();
		}

		m_pWindow->clear();
		render();
		m_pWindow->display();
	}
}

void CheckersGameEngine::processEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		m_pWindow->close();
		break;
	default:
	{
		Gui* gui = m_gui->processEvent(m_pWindow, event);
		if (gui != nullptr)
		{
			delete m_gui;
			m_gui = gui;
		}
	}
	}
}

void CheckersGameEngine::render()
{
	renderer::render(m_pWindow, m_background);
	// renderer::render(m_pWindow, m_boardRect, sf::Color::Red);
	renderer::render(m_pWindow, m_boardRect, sf::Color(232, 208, 170, 255));
	for (const CheckerSquare& square : m_squares)
	{
		//renderer::render(m_pWindow, square.getRect(), sf::Color::Black);
		renderer::render(m_pWindow, square);
	}

	m_gui->render(m_pWindow);

	/*switch (m_guiState)
	{
	case kMainMenu:

		break;
	case kPlaying:
		renderer::render(m_pWindow, m_background);
		renderer::render(m_pWindow, m_boardRect, sf::Color::Red);
		break;
	case kGameOverMenu:
		break;
	}*/
}

void CheckersGameEngine::applyTextures()
{
	m_background.setTexture(resources::g_textures.at("background"));
}

// Initial game state:
//
// . R . R . R . R
// R . R . R . R .
// . R . R . R . R
// X . X . X . X .
// . X . X . X . X
// B . B . B . B .
// . B . B . B . B
// B . B . B . B .
//
// R = red player's piece
// B = black player's piece
// X = empty black square
// . = empty red square (ignored)
//
// The black squares are indexed as such:
//
//  .  0  .  1  .  2  .  3
//  4  .  5  .  6  .  7  .
//  .  8  .  9  . 10  . 11
// 12  . 13  . 14  . 15  .
//  . 16  . 17  . 18  . 19
// 20  . 21  . 22  . 23  .
//  . 24  . 25  . 26  . 27
// 28  . 29  . 30  . 31  .
void CheckersGameEngine::initializeGameState()
{
	// Create players (TODO: allow other player configurations)
	m_gameState.currentPlayer = 0;
	m_gameState.players.at(0) = new HumanPlayer(kBlack);
	m_gameState.players.at(1) = new HumanPlayer(kRed);

	// Create squares
	float squareWidth = 100.0f;
	for (std::size_t i = 0; i < 32; ++i)
	{
		std::size_t row = i / 4;
		std::size_t col = i % 4;
		bool isFirstSquareInRowBlack = row % 2;
		float offsetFromLeft = squareWidth * col * 2.0f + (isFirstSquareInRowBlack ? 0.0f : squareWidth);
		float offsetFromTop = squareWidth * row;
		CheckerSquare square({
			m_boardRect.left + offsetFromLeft,
			m_boardRect.top + offsetFromTop,
			squareWidth,
			squareWidth
			});
		m_squares.push_back(square);
	}

	// Create pieces
	for (std::size_t i = 0; i < 24; ++i)
	{
		CheckerColor color = i < 12 ? kRed : kBlack;
		CheckerPiece piece(color);
		m_pieces.push_back(piece);
	}

	// Place pieces on squares
	for (std::size_t i = 0; i < 32; ++i)
	{
		if (i < 12)
		{
			m_squares[i].placePiece(&m_pieces[i]);
		}
		else if (i >= 20)
		{
			m_squares[i].placePiece(&m_pieces[i - 8]);
		}
	}

	// Set up game state
	for (auto& square : m_squares)
	{
		m_gameState.squares.push_back(&square);
	}
	for (auto& piece : m_pieces)
	{
		m_gameState.pieces.push_back(&piece);
	}
}