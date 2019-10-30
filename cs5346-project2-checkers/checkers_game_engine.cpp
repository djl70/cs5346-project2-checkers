#include "checkers_game_engine.h"

#include "config.h"
#include "resource_manager.h"

#include "base_state.h"
#include "main_menu_state.h"

CheckersGameEngine::CheckersGameEngine()
	: m_window(sf::VideoMode(config::kScreenWidth, config::kScreenHeight), "Checkers", sf::Style::Titlebar | sf::Style::Close)
	, m_pState{ nullptr }
{
	m_window.setFramerateLimit(config::kFps);

	m_resources.loadResources(&m_window);

	// m_pState = new CheckersGameState;
	m_pState = new MainMenuState{ &m_resources };
	m_pState->enter();
}

CheckersGameEngine::~CheckersGameEngine()
{
	if (m_pState)
	{
		m_pState->exit();
		delete m_pState;
		m_pState = nullptr;
	}

	m_resources.freeResources();
}

void CheckersGameEngine::run()
{
	while (m_resources.getWindow()->isOpen())
	{
		m_resources.update();

		BaseState* pNextState = m_pState->event();
		m_pState->render();

		if (pNextState)
		{
			m_pState->exit();
			delete m_pState;
			m_pState = pNextState;
			m_pState->enter();
		}
	}
}