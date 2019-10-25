#include "renderer.h"

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
}