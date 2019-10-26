#pragma once

#include <SFML/Graphics.hpp>

class CheckerPiece;
class CheckerSquare;

namespace renderer
{
	// Generic
	void render(sf::RenderWindow* pWindow, const sf::Drawable& drawable);
	// Rectangle
	void render(sf::RenderWindow* pWindow, const sf::FloatRect& rect, const sf::Color& color);
	// Circle
	void render(sf::RenderWindow* pWindow, const sf::Vector2f& position, float radius, const sf::Color& color);
	// Checker piece
	void render(sf::RenderWindow* pWindow, const CheckerPiece& piece, const sf::Vector2f& position, float radius);
	// Checker square + its piece
	void render(sf::RenderWindow* pWindow, const CheckerSquare& square);
}