#include "button.h"

#include "config.h"
#include "resources.h"

Button::Button(const sf::FloatRect& rect, const sf::Texture& normal, const sf::Texture& hover, const sf::Texture& press)
	: m_state{ kDefault }
	, m_rect{ rect }
	, m_kDefaultTexture{ normal }
	, m_kHoveredTexture{ hover }
	, m_kPressedTexture{ press }
{
	m_sprite.setTexture(m_kDefaultTexture);
	m_sprite.setPosition({ m_rect.left, m_rect.top });
	m_sprite.setScale({ config::kScaling, config::kScaling });
}

bool Button::update(const sf::Event& event)
{
	bool isButtonClicked = false;
	sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*resources::pWindow));

	// Default state can transition to hovered if mouse position is over the button
	// Default state can transition to pressed if mouse position is over the button AND left mouse button is pressed
	if (m_state == kDefault)
	{
		if (m_rect.contains(mousePosition))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				changeState(kPressed);
			}
			else
			{
				changeState(kHovered);
			}
		}
	}
	// Hovered state can transition to default if mouse position is not over the button
	// Hovered state can transition to pressed if mouse position is over the button AND left mouse button is pressed
	else if (m_state == kHovered)
	{
		if (m_rect.contains(mousePosition))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				changeState(kPressed);
			}
		}
		else
		{
			changeState(kDefault);
		}
	}
	// Pressed state can transition to default if mouse position is not over the button
	// Pressed state can transition to hovered if mouse position is over the button AND left mouse button not pressed (sets clicked to true)
	else if (m_state == kPressed)
	{
		if (m_rect.contains(mousePosition))
		{
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				changeState(kHovered);
				isButtonClicked = true;
			}
		}
		else
		{
			changeState(kDefault);
		}
	}

	return isButtonClicked;
}

void Button::render(sf::RenderWindow* pWindow) const
{
	pWindow->draw(m_sprite);
}

void Button::changeState(ButtonState newState)
{
	m_state = newState;
	switch (newState)
	{
	case kDefault:
		m_sprite.setTexture(m_kDefaultTexture);
		break;
	case kHovered:
		m_sprite.setTexture(m_kHoveredTexture);
		break;
	case kPressed:
		m_sprite.setTexture(m_kPressedTexture);
		break;
	}
}