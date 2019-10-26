#include "renderer.h"

#include "checker_piece.h"
#include "checker_square.h"

namespace renderer
{
	void render(sf::RenderWindow* pWindow, const sf::Drawable& drawable)
	{
		pWindow->draw(drawable);
	}

	void render(sf::RenderWindow* pWindow, const sf::FloatRect& rect, const sf::Color& color)
	{
		sf::RectangleShape shape;
		shape.setFillColor(color);
		shape.setSize({ rect.width, rect.height });
		shape.setPosition({ rect.left, rect.top });
		render(pWindow, shape);
	}

	void render(sf::RenderWindow* pWindow, const sf::Vector2f& position, float radius, const sf::Color& color)
	{
		sf::CircleShape shape;
		shape.setFillColor(color);
		shape.setRadius(radius);
		shape.setPosition(position);
		render(pWindow, shape);
	}

	void render(sf::RenderWindow* pWindow, const CheckerPiece& piece, const sf::Vector2f& position, float radius)
	{
		sf::Color color;
		switch (piece.getColor())
		{
		case kBlack:
			color = sf::Color(107, 73, 63, 255);
			break;
		case kRed:
			color = sf::Color(244, 200, 69, 255);
			break;
		}
		render(pWindow, position, radius, color);
	}

	void render(sf::RenderWindow* pWindow, const CheckerSquare& square)
	{
		sf::FloatRect rect = square.getRect();
		// render(pWindow, rect, sf::Color::Black);
		render(pWindow, rect, sf::Color(166, 125, 93, 255));
		CheckerPiece* piece = square.getPiece();
		if (piece)
		{
			float radius = 0.45f * rect.width;
			render(pWindow, *piece, { rect.left + 0.5f * (rect.width - 2.0f * radius), rect.top + 0.5f * (rect.height - 2.0f * radius) }, radius);
		}
	}
}