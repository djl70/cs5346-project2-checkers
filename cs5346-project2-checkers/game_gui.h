#pragma once

#include "gui.h"

class GameGui : public Gui
{
public:
	Gui* processEvent(sf::RenderWindow* pWindow, const sf::Event& event) override;
	void render(sf::RenderWindow* pWindow) override;

private:

};