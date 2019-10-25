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
	renderer::render(m_pWindow, m_boardRect, sf::Color::Red);

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