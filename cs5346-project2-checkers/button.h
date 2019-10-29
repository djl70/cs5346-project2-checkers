#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Button
{
public:
	Button(const sf::FloatRect& rect, const sf::Texture& normal, const sf::Texture& hover, const sf::Texture& press, const sf::SoundBuffer& clickSound);

	bool update(const sf::Event& event, const sf::Vector2f& mousePositionInWindow);

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
	const sf::SoundBuffer& m_soundBuffer;
	sf::Sound m_clickSound;

	void changeState(ButtonState newState);
};