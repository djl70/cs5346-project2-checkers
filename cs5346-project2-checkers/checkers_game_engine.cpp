#include "checkers_game_engine.h"

#include "config.h"
#include "resources.h"

#include "base_state.h"
#include "main_menu_state.h"

CheckersGameEngine::CheckersGameEngine(unsigned int fps)
	: m_window(sf::VideoMode(config::kScreenWidth, config::kScreenHeight), "Checkers", sf::Style::Titlebar | sf::Style::Close)
	, m_pState{ nullptr }
{
	resources::loadResources();

	m_window.setFramerateLimit(fps);
	resources::pWindow = &m_window;

	// m_pState = new CheckersGameState;
	m_pState = new MainMenuState;
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
}

void CheckersGameEngine::run()
{
	while (resources::pWindow->isOpen())
	{
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

	if (m_pState)
	{
		m_pState->exit();
		delete m_pState;
		m_pState = nullptr;
	}
}