#include "main_menu.h"

#include "game_gui.h"
#include "renderer.h"
#include "resources.h"

MainMenu::MainMenu()
{
	m_playButton.setFillColor(sf::Color::White);
	m_playButton.setOutlineColor(sf::Color::Black);
	// m_playButton.setPosition({ 760.0f, 440.0f });
	m_playButton.setSize({ 400.0f, 200.0f });
	m_playButton.setOrigin({ 200.0f, 100.0f });
	m_playButton.setPosition({ 960.0f, 540.0f });

	m_playButtonText.setFont(resources::g_fonts.at("arial"));
	m_playButtonText.setString("Play 1P vs 2P");
	m_playButtonText.setCharacterSize(48);
	m_playButtonText.setFillColor(sf::Color::Black);

	sf::FloatRect textRect = m_playButtonText.getLocalBounds();
	m_playButtonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	m_playButtonText.setPosition(m_playButton.getPosition());
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