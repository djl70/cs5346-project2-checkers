#include "main_menu_state.h"

#include "checkers_game_state.h"
#include "config.h"
#include "resources.h"

MainMenuState::MainMenuState()
	: m_singlePlayerButton{ config::singlePlayerButtonRect, resources::textures["button_singleplayer"], resources::textures["button_singleplayer_hover"], resources::textures["button_singleplayer_press"] }
	, m_multiPlayerButton{ config::multiPlayerButtonRect, resources::textures["button_multiplayer"], resources::textures["button_multiplayer_hover"], resources::textures["button_multiplayer_press"] }
	, m_autoPlayButton{ config::autoPlayButtonRect, resources::textures["button_autoplay"], resources::textures["button_autoplay_hover"], resources::textures["button_autoplay_press"] }
{
	m_title.setPosition({ config::titleRect.left, config::titleRect.top });
	m_title.setTexture(resources::textures["title"]);
	m_title.setScale({ config::kScaling, config::kScaling });

	m_background.setTexture(resources::textures["plain_background"]);
	m_background.setScale({ config::kScaling, config::kScaling });

	m_buttonClick.setBuffer(resources::sounds["sound_move"]);
}

void MainMenuState::enter()
{

}

BaseState* MainMenuState::event()
{
	sf::Event event;
	while (resources::pWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			resources::pWindow->close();
			break;
		}

		if (m_singlePlayerButton.update(event))
		{
			m_buttonClick.play();
			return new CheckersGameState;
		}
		else if (m_multiPlayerButton.update(event))
		{
			m_buttonClick.play();
			return new CheckersGameState;
		}
		else if (m_autoPlayButton.update(event))
		{
			m_buttonClick.play();
			return new CheckersGameState;
		}
	}

	return nullptr;
}

void MainMenuState::render()
{
	resources::pWindow->clear(sf::Color::White);

	resources::pWindow->draw(m_background);

	// Draw title and buttons
	resources::pWindow->draw(m_title);
	m_singlePlayerButton.render(resources::pWindow);
	m_multiPlayerButton.render(resources::pWindow);
	m_autoPlayButton.render(resources::pWindow);

	resources::pWindow->display();
}

void MainMenuState::exit()
{
	// We need this so that the sound will finish playing before it is destroyed
	while (m_buttonClick.getStatus() == sf::Sound::Status::Playing)
	{

	}

	// If we don't include this, the program will throw an exception on close
	m_buttonClick.resetBuffer();
}