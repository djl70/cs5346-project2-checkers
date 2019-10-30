#include "game_over_state.h"

#include "config.h"
#include "main_menu_state.h"

GameOverState::GameOverState(ResourceManager* pResources, CheckerColor winningColor)
	: m_pResources{ pResources }
	, m_piece{ winningColor, winningColor == kRed ? m_pResources->getTexture("red_man") : m_pResources->getTexture("black_man"), winningColor == kRed ? m_pResources->getTexture("red_king") : m_pResources->getTexture("black_king") }
{
	m_piece.promote();
	m_piece.setPosition({ config::kScreenWidth * 0.5f - config::kSquareWidth * 0.5f, config::kScreenHeight * 0.5f - config::kSquareWidth * 0.5f });

	m_menuButton.setRect(config::menuButtonRect);
	m_menuButton.setTexture(kDefault, m_pResources->getTexture("button_menu"));
	m_menuButton.setTexture(kHovered, m_pResources->getTexture("button_menu_hover"));
	m_menuButton.setTexture(kPressed, m_pResources->getTexture("button_menu_press"));
	//m_menuButton.setClickSound(m_pResources->getSoundBuffer("sound_move"));

	m_background.setTexture(*m_pResources->getTexture("plain_background"));
	m_background.setScale({ config::kScaling, config::kScaling });

	m_winnerText.setTexture(*m_pResources->getTexture("winner"));
	m_winnerText.setScale({ config::kScaling, config::kScaling });
	m_winnerText.setPosition({ config::titleRect.left, config::titleRect.top });
}

void GameOverState::enter()
{

}

BaseState* GameOverState::event()
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

		if (m_menuButton.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
			return new MainMenuState{ m_pResources };
		}
	}

	return nullptr;
}

void GameOverState::render()
{
	m_pResources->getWindow()->clear();

	m_pResources->getWindow()->draw(m_background);
	m_pResources->getWindow()->draw(m_winnerText);
	m_piece.render(m_pResources->getWindow());
	m_menuButton.render(m_pResources->getWindow());

	m_pResources->getWindow()->display();
}

void GameOverState::exit()
{

}