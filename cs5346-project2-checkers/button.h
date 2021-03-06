#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

enum ButtonState
{
	kDefault,
	kHovered,
	kPressed
};

class Button
{
public:
	Button();
	~Button();

	void setRect(const sf::FloatRect& rect);
	void setTexture(ButtonState state, sf::Texture* texture);
	void setEnabled(bool enabled);

	virtual bool update(const sf::Event& event, const sf::Vector2f& mousePositionInWindow);
	virtual void render(sf::RenderWindow* pWindow) const;

protected:
	ButtonState m_state;
	sf::FloatRect m_rect;
	sf::Texture* m_defaultTexture;
	sf::Texture* m_hoveredTexture;
	sf::Texture* m_pressedTexture;
	sf::Sprite m_sprite;
	bool m_enabled;

	void changeState(ButtonState newState);
};