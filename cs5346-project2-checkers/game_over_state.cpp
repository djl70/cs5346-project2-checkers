#include "game_over_state.h"

#include "config.h"
#include "main_menu_state.h"

GameOverState::GameOverState(ResourceManager& resources, CheckerColor winningColor)
	: m_resources{ resources }
	, m_piece{ winningColor, winningColor == kRed ? m_resources.getTexture("red_man") : m_resources.getTexture("black_man"), winningColor == kRed ? m_resources.getTexture("red_king") : m_resources.getTexture("black_king") }
	, m_menuButton{ config::menuButtonRect, m_resources.getTexture("button_menu"), m_resources.getTexture("button_menu_hover"), m_resources.getTexture("button_menu_press"), m_resources.getSound("sound_move") }
{
	m_piece.promote();
	m_piece.setPosition({ config::kScreenWidth * 0.5f - config::kSquareWidth * 0.5f, config::kScreenHeight * 0.5f - config::kSquareWidth * 0.5f });

	m_background.setTexture(m_resources.getTexture("plain_background"));
	m_background.setScale({ config::kScaling, config::kScaling });

	m_winnerText.setTexture(m_resources.getTexture("winner"));
	m_winnerText.setScale({ config::kScaling, config::kScaling });
	m_winnerText.setPosition({ config::titleRect.left, config::titleRect.top });
}

void GameOverState::enter()
{

}

BaseState* GameOverState::event()
{
	sf::Vector2f mousePositionInWindow = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_resources.getWindow()));

	sf::Event event;
	while (m_resources.getWindow()->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_resources.getWindow()->close();
			break;
		}

		if (m_menuButton.update(event, mousePositionInWindow))
		{
			return new MainMenuState(m_resources);
		}
	}

	return nullptr;
}

void GameOverState::render()
{
	m_resources.getWindow()->clear();

	m_resources.getWindow()->draw(m_background);
	m_resources.getWindow()->draw(m_winnerText);
	m_piece.render(m_resources.getWindow());
	m_menuButton.render(m_resources.getWindow());

	m_resources.getWindow()->display();
}

void GameOverState::exit()
{

}