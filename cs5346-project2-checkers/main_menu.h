#pragma once

#include "gui.h"

#include <SFML/Graphics.hpp>

class MainMenu : public Gui
{
public:
	MainMenu();

	Gui* processEvent(sf::RenderWindow* pWindow, const sf::Event& event) override;
	void render(sf::RenderWindow* pWindow) override;

private:
	sf::RectangleShape m_playButton;
	sf::Text m_playButtonText;
};