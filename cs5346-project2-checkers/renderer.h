#pragma once

#include "checkerboard.h"

#include <SFML/Graphics.hpp>

namespace renderer
{
	void render(sf::RenderWindow* pWindow, const sf::Drawable& drawable);
	void render(sf::RenderWindow* pWindow, const sf::FloatRect& rect, const sf::Color& color);
}