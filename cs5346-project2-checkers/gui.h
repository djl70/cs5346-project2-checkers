#pragma once

#include "checkers_game_state.h"

#include <SFML/Graphics.hpp>

class Gui
{
public:
	virtual Gui* processEvent(sf::RenderWindow* pWindow, const sf::Event& event) = 0;
	virtual void render(sf::RenderWindow* pWindow) = 0;
	//virtual void update(CheckersGameState& gameState) = 0;

private:

};