#include "player_configuration_menu_state.h"

#include "checkers_game_state.h"
#include "config.h"
#include "main_menu_state.h"

#include "ai_player.h"
#include "human_player.h"

PlayerConfigurationMenuState::PlayerConfigurationMenuState(ResourceManager* pResources, unsigned int aiPlayerCount)
	: m_pResources{ pResources }
	, m_aiPlayerCount{ aiPlayerCount }
{
	
}

void PlayerConfigurationMenuState::enter()
{
	// TODO: If P1 vs COM, allow the choice of moving first or second

	if (m_aiPlayerCount == 0)
	{
		return;
	}

	ToggleButton button;

	// Left-side buttons
	button.setRect({ config::leftAlgorithmButtonGroupPosition.x, config::leftAlgorithmButtonGroupPosition.y - config::menuButtonSize.y * 0.5f, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_minimax"));
	button.setTexture(kHovered, m_pResources->getTexture("button_minimax_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_minimax_press"));
	m_leftAlgorithmButtons.addButton(button);

	button.setRect({ config::leftAlgorithmButtonGroupPosition.x, config::leftAlgorithmButtonGroupPosition.y + config::menuButtonSize.y * 0.5f, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_alphabeta"));
	button.setTexture(kHovered, m_pResources->getTexture("button_alphabeta_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_alphabeta_press"));
	m_leftAlgorithmButtons.addButton(button);

	button.setRect({ config::leftHeuristicButtonGroupPosition.x, config::leftHeuristicButtonGroupPosition.y - config::menuButtonSize.y * 0.5f, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_h1"));
	button.setTexture(kHovered, m_pResources->getTexture("button_h1_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_h1_press"));
	m_leftHeuristicButtons.addButton(button);

	button.setRect({ config::leftHeuristicButtonGroupPosition.x, config::leftHeuristicButtonGroupPosition.y + config::menuButtonSize.y * 0.5f, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_h2"));
	button.setTexture(kHovered, m_pResources->getTexture("button_h2_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_h2_press"));
	m_leftHeuristicButtons.addButton(button);

	// Right-side buttons
	button.setRect({ config::rightAlgorithmButtonGroupPosition.x, config::rightAlgorithmButtonGroupPosition.y - config::menuButtonSize.y * 0.5f, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_minimax"));
	button.setTexture(kHovered, m_pResources->getTexture("button_minimax_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_minimax_press"));
	m_rightAlgorithmButtons.addButton(button);

	button.setRect({ config::rightAlgorithmButtonGroupPosition.x, config::rightAlgorithmButtonGroupPosition.y + config::menuButtonSize.y * 0.5f, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_alphabeta"));
	button.setTexture(kHovered, m_pResources->getTexture("button_alphabeta_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_alphabeta_press"));
	m_rightAlgorithmButtons.addButton(button);

	button.setRect({ config::rightHeuristicButtonGroupPosition.x, config::rightHeuristicButtonGroupPosition.y - config::menuButtonSize.y * 0.5f, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_h1"));
	button.setTexture(kHovered, m_pResources->getTexture("button_h1_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_h1_press"));
	m_rightHeuristicButtons.addButton(button);

	button.setRect({ config::rightHeuristicButtonGroupPosition.x, config::rightHeuristicButtonGroupPosition.y + config::menuButtonSize.y * 0.5f, config::menuButtonSize.x, config::menuButtonSize.y });
	button.setTexture(kDefault, m_pResources->getTexture("button_h2"));
	button.setTexture(kHovered, m_pResources->getTexture("button_h2_hover"));
	button.setTexture(kPressed, m_pResources->getTexture("button_h2_press"));
	m_rightHeuristicButtons.addButton(button);

	if (m_aiPlayerCount < 2)
	{
		m_rightAlgorithmButtons.setEnabled(false);
		m_rightHeuristicButtons.setEnabled(false);
	}

	m_startGameButton.setRect(config::startButtonRect);
	m_startGameButton.setTexture(kDefault, m_pResources->getTexture("button_start"));
	m_startGameButton.setTexture(kHovered, m_pResources->getTexture("button_start_hover"));
	m_startGameButton.setTexture(kPressed, m_pResources->getTexture("button_start_press"));
	
	m_mainMenuButton.setRect(config::menuButtonRect);
	m_mainMenuButton.setTexture(kDefault, m_pResources->getTexture("button_menu"));
	m_mainMenuButton.setTexture(kHovered, m_pResources->getTexture("button_menu_hover"));
	m_mainMenuButton.setTexture(kPressed, m_pResources->getTexture("button_menu_press"));
	
	m_background.setTexture(*m_pResources->getTexture("plain_background"));
	m_background.setScale({ config::kScaling, config::kScaling });
}

BaseState* PlayerConfigurationMenuState::event()
{
	if (m_aiPlayerCount == 0)
	{
		return new CheckersGameState{ m_pResources, new HumanPlayer{ kBlack }, new HumanPlayer{ kRed } };
	}

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

		if (m_leftAlgorithmButtons.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
		}
		
		if (m_leftHeuristicButtons.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
		}

		if (m_rightAlgorithmButtons.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
		}

		if (m_rightHeuristicButtons.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
		}

		if (m_startGameButton.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
			if (m_aiPlayerCount == 1)
			{
				return new CheckersGameState{ m_pResources, new HumanPlayer{ kBlack }, new AIPlayer{ kRed } };
			}
			else if (m_aiPlayerCount == 2)
			{
				return new CheckersGameState{ m_pResources, new AIPlayer{ kBlack }, new AIPlayer{ kRed } };
			}
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
	if (m_aiPlayerCount == 0)
	{
		return;
	}

	m_pResources->getWindow()->clear(sf::Color::White);

	m_pResources->getWindow()->draw(m_background);

	m_leftAlgorithmButtons.render(m_pResources->getWindow());
	m_leftHeuristicButtons.render(m_pResources->getWindow());
	m_rightAlgorithmButtons.render(m_pResources->getWindow());
	m_rightHeuristicButtons.render(m_pResources->getWindow());
	m_startGameButton.render(m_pResources->getWindow());
	m_mainMenuButton.render(m_pResources->getWindow());

	m_pResources->getWindow()->display();
}

void PlayerConfigurationMenuState::exit()
{

}