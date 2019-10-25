#include "main_menu.h"

#include "game_gui.h"
#include "renderer.h"
#include "resources.h"

MainMenu::MainMenu()
{
	m_playButton.setFillColor(sf::Color::White);
	m_playButton.setOutlineColor(sf::Color::Black);
	m_playButton.setPosition({ 760.0f, 440.0f });
	m_playButton.setSize({ 400.0f, 200.0f });
	m_playButtonText.setFillColor(sf::Color::Black);
	m_playButtonText.setFont(resources::g_fonts.at("arial"));
	m_playButtonText.setPosition(m_playButton.getPosition());
	m_playButtonText.setCharacterSize(48);
}

Gui* MainMenu::processEvent(sf::RenderWindow* pWindow, const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Button::Left)
		{
			if (m_playButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
			{
				return new GameGui;
			}
		}
	}

	return nullptr;
}

void MainMenu::render(sf::RenderWindow* pWindow)
{
	renderer::render(pWindow, m_playButton);
	renderer::render(pWindow, m_playButtonText);
}