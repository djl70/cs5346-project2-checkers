#pragma once

#include <SFML/Graphics.hpp>

class Button
{
public:
	Button(const sf::FloatRect& rect, const sf::Texture& normal, const sf::Texture& hover, const sf::Texture& press);

	bool update(const sf::Event& event);

	void render(sf::RenderWindow* pWindow) const;

private:
	enum ButtonState
	{
		kDefault,
		kHovered,
		kPressed
	};

	ButtonState m_state;
	sf::FloatRect m_rect;
	const sf::Texture& m_kDefaultTexture;
	const sf::Texture& m_kHoveredTexture;
	const sf::Texture& m_kPressedTexture;
	sf::Sprite m_sprite;

	void changeState(ButtonState newState);
};