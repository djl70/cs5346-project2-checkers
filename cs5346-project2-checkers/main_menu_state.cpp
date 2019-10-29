#include "main_menu_state.h"

#include "checkers_game_state.h"
#include "config.h"

MainMenuState::MainMenuState(ResourceManager& resources)
	: m_resources{ resources }
	, m_singlePlayerButton{ config::singlePlayerButtonRect, m_resources.getTexture("button_singleplayer"), m_resources.getTexture("button_singleplayer_hover"), m_resources.getTexture("button_singleplayer_press"), m_resources.getSound("sound_move") }
	, m_multiPlayerButton{ config::multiPlayerButtonRect, m_resources.getTexture("button_multiplayer"), m_resources.getTexture("button_multiplayer_hover"), m_resources.getTexture("button_multiplayer_press"), m_resources.getSound("sound_move") }
	, m_autoPlayButton{ config::autoPlayButtonRect, m_resources.getTexture("button_autoplay"), m_resources.getTexture("button_autoplay_hover"), m_resources.getTexture("button_autoplay_press"), m_resources.getSound("sound_move") }
{
	m_title.setPosition({ config::titleRect.left, config::titleRect.top });
	m_title.setTexture(m_resources.getTexture("title"));
	m_title.setScale({ config::kScaling, config::kScaling });

	m_background.setTexture(m_resources.getTexture("plain_background"));
	m_background.setScale({ config::kScaling, config::kScaling });

	//m_buttonClick.setBuffer(resources::sounds["sound_move"]);
}

void MainMenuState::enter()
{

}

BaseState* MainMenuState::event()
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

		if (m_singlePlayerButton.update(event, mousePositionInWindow))
		{
			//m_buttonClick.play();
			return new CheckersGameState(m_resources);
		}
		else if (m_multiPlayerButton.update(event, mousePositionInWindow))
		{
			//m_buttonClick.play();
			return new CheckersGameState(m_resources);
		}
		else if (m_autoPlayButton.update(event, mousePositionInWindow))
		{
			//m_buttonClick.play();
			return new CheckersGameState(m_resources);
		}
	}

	return nullptr;
}

void MainMenuState::render()
{
	m_resources.getWindow()->clear(sf::Color::White);

	m_resources.getWindow()->draw(m_background);

	// Draw title and buttons
	m_resources.getWindow()->draw(m_title);
	m_singlePlayerButton.render(m_resources.getWindow());
	m_multiPlayerButton.render(m_resources.getWindow());
	m_autoPlayButton.render(m_resources.getWindow());

	m_resources.getWindow()->display();
}

void MainMenuState::exit()
{
	/*
	// We need this so that the sound will finish playing before it is destroyed
	while (m_buttonClick.getStatus() == sf::Sound::Status::Playing)
	{

	}

	// If we don't include this, the program will throw an exception on close
	m_buttonClick.resetBuffer();*/
}