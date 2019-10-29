#include "game_over_state.h"

#include "config.h"
#include "main_menu_state.h"
#include "resources.h"

GameOverState::GameOverState(CheckerColor winningColor)
	: m_piece{ winningColor }
{
	m_piece.promote();
	m_piece.setPosition({ config::kScreenWidth * 0.5f - config::kSquareWidth * 0.5f, config::kScreenHeight * 0.5f - config::kSquareWidth * 0.5f });

	m_background.setTexture(resources::textures["plain_background"]);
	m_background.setScale({ config::kScaling, config::kScaling });
}

void GameOverState::enter()
{

}

BaseState* GameOverState::event()
{
	sf::Event event;
	while (resources::pWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			resources::pWindow->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape)
			{
				return new MainMenuState;
			}
			break;
		}
	}

	return nullptr;
}

void GameOverState::render()
{
	resources::pWindow->clear();

	resources::pWindow->draw(m_background);
	m_piece.render(resources::pWindow);

	resources::pWindow->display();
}

void GameOverState::exit()
{

}