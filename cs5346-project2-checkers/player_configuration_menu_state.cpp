#include "player_configuration_menu_state.h"

#include "checkers_game_state.h"
#include "config.h"
#include "main_menu_state.h"

PlayerConfigurationMenuState::PlayerConfigurationMenuState(ResourceManager* pResources, unsigned int aiPlayerCount)
	: m_pResources{ pResources }
	, m_aiPlayerCount{ aiPlayerCount }
{
	
}

void PlayerConfigurationMenuState::enter()
{
	// TODO: Set up GUI with buttons.
	// If aiPlayerCount == 0, return immediately.
	// If aiPlayerCount >= 1, enable buttons on the left side
	// If aiPlayerCount == 2, enable buttons on the right side

	ToggleButton button;

	button.setRect({ config::multiPlayerButtonRect.left - config::menuButtonSize.x * 0.5f, config::multiPlayerButtonRect.top, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_minimax"));
	button.setTexture(kHovered, m_pResources->getTexture("button_minimax_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_minimax_press"));
	//button.setClickSound(m_pResources->getSoundBuffer("sound_move"));
	m_algorithmButtons.addButton(button);

	button.setRect({ config::multiPlayerButtonRect.left + config::menuButtonSize.x * 0.5f, config::multiPlayerButtonRect.top, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_alphabeta"));
	button.setTexture(kHovered, m_pResources->getTexture("button_alphabeta_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_alphabeta_press"));
	//button.setClickSound(m_pResources->getSoundBuffer("sound_move"));
	m_algorithmButtons.addButton(button);

	button.setRect({ config::singlePlayerButtonRect.left - config::menuButtonSize.x * 0.5f, config::singlePlayerButtonRect.top, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_h1"));
	button.setTexture(kHovered, m_pResources->getTexture("button_h1_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_h1_press"));
	//button.setClickSound(m_pResources->getSoundBuffer("sound_move"));
	m_heuristicButtons.addButton(button);

	button.setRect({ config::singlePlayerButtonRect.left + config::menuButtonSize.x * 0.5f, config::singlePlayerButtonRect.top, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_h2"));
	button.setTexture(kHovered, m_pResources->getTexture("button_h2_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_h2_press"));
	//button.setClickSound(m_pResources->getSoundBuffer("sound_move"));
	m_heuristicButtons.addButton(button);

	m_startGameButton.setRect(config::autoPlayButtonRect);
	m_startGameButton.setTexture(kDefault, m_pResources->getTexture("button_start"));
	m_startGameButton.setTexture(kHovered, m_pResources->getTexture("button_start_hover"));
	m_startGameButton.setTexture(kPressed, m_pResources->getTexture("button_start_press"));
	//m_startGameButton.setClickSound(m_pResources->getSoundBuffer("sound_move"));

	m_mainMenuButton.setRect(config::menuButtonRect);
	m_mainMenuButton.setTexture(kDefault, m_pResources->getTexture("button_menu"));
	m_mainMenuButton.setTexture(kHovered, m_pResources->getTexture("button_menu_hover"));
	m_mainMenuButton.setTexture(kPressed, m_pResources->getTexture("button_menu_press"));
	//m_mainMenuButton.setClickSound(m_pResources->getSoundBuffer("sound_move"));

	m_background.setTexture(*m_pResources->getTexture("plain_background"));
	m_background.setScale({ config::kScaling, config::kScaling });
}

BaseState* PlayerConfigurationMenuState::event()
{
	sf::Vector2f mousePositionInWindow = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_pResources->getWindow()));

	sf::Event event;
	while (m_pResources->getWindow()->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_pResources->getWindow()->close();
			break;
		}

		if (m_algorithmButtons.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
		}
		
		if (m_heuristicButtons.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
		}

		if (m_startGameButton.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
			return new CheckersGameState{ m_pResources };
		}

		if (m_mainMenuButton.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
			return new MainMenuState{ m_pResources };
		}
	}

	return nullptr;
}

void PlayerConfigurationMenuState::render()
{
	m_pResources->getWindow()->clear(sf::Color::White);

	m_pResources->getWindow()->draw(m_background);

	m_algorithmButtons.render(m_pResources->getWindow());
	m_heuristicButtons.render(m_pResources->getWindow());
	m_startGameButton.render(m_pResources->getWindow());
	m_mainMenuButton.render(m_pResources->getWindow());

	m_pResources->getWindow()->display();
}

void PlayerConfigurationMenuState::exit()
{

}