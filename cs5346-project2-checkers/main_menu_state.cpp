#include "main_menu_state.h"

#include "checkers_game_state.h"
#include "config.h"
#include "player_configuration_menu_state.h"
#include "simulation_state.h"

MainMenuState::MainMenuState(ResourceManager* resources)
	: m_pResources{ resources }
{
	m_multiPlayerButton.setRect(config::multiPlayerButtonRect);
	m_multiPlayerButton.setTexture(kDefault, m_pResources->getTexture("button_multiplayer"));
	m_multiPlayerButton.setTexture(kHovered, m_pResources->getTexture("button_multiplayer_hover"));
	m_multiPlayerButton.setTexture(kPressed, m_pResources->getTexture("button_multiplayer_press"));
	//m_multiPlayerButton.setClickSound(m_pResources->getSoundBuffer("sound_move"));
	
	m_singlePlayerButton.setRect(config::singlePlayerButtonRect);
	m_singlePlayerButton.setTexture(kDefault, m_pResources->getTexture("button_singleplayer"));
	m_singlePlayerButton.setTexture(kHovered, m_pResources->getTexture("button_singleplayer_hover"));
	m_singlePlayerButton.setTexture(kPressed, m_pResources->getTexture("button_singleplayer_press"));
	//m_singlePlayerButton.setClickSound(m_pResources->getSoundBuffer("sound_move"));

	m_autoPlayButton.setRect(config::autoPlayButtonRect);
	m_autoPlayButton.setTexture(kDefault, m_pResources->getTexture("button_autoplay"));
	m_autoPlayButton.setTexture(kHovered, m_pResources->getTexture("button_autoplay_hover"));
	m_autoPlayButton.setTexture(kPressed, m_pResources->getTexture("button_autoplay_press"));
	//m_autoPlayButton.setClickSound(m_pResources->getSoundBuffer("sound_move"));

	m_title.setPosition({ config::titleRect.left, config::titleRect.top });
	m_title.setTexture(*m_pResources->getTexture("title"));
	m_title.setScale({ config::kScaling, config::kScaling });

	m_background.setTexture(*m_pResources->getTexture("plain_background"));
	m_background.setScale({ config::kScaling, config::kScaling });
}

MainMenuState::~MainMenuState() {

}

void MainMenuState::enter()
{

}

BaseState* MainMenuState::event()
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

		
		if (m_multiPlayerButton.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
			return new PlayerConfigurationMenuState{ m_pResources, 0 };
		}

		if (m_singlePlayerButton.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
			return new PlayerConfigurationMenuState{ m_pResources, 1 };
		}
		
		if (m_autoPlayButton.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
			//return new PlayerConfigurationMenuState{ m_pResources, 2 };
			return new SimulationState{ m_pResources };
		}
	}

	return nullptr;
}

void MainMenuState::render()
{
	m_pResources->getWindow()->clear(sf::Color::White);

	m_pResources->getWindow()->draw(m_background);

	// Draw title and buttons
	m_pResources->getWindow()->draw(m_title);

	m_multiPlayerButton.render(m_pResources->getWindow());
	m_singlePlayerButton.render(m_pResources->getWindow());
	m_autoPlayButton.render(m_pResources->getWindow());

	m_pResources->getWindow()->display();
}