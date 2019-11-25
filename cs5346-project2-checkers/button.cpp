#include "button.h"

#include "config.h"

Button::Button()
	: m_state{ kDefault }
	, m_enabled{ true }
{
	m_sprite.setScale({ config::kScaling, config::kScaling });
}

Button::~Button()
{

}

void Button::setRect(const sf::FloatRect& rect)
{
	m_rect = rect;
	m_sprite.setPosition({ m_rect.left, m_rect.top });
}

void Button::setTexture(ButtonState state, sf::Texture* texture)
{
	switch (state)
	{
	case kDefault:
		m_defaultTexture = texture;
		if (!m_sprite.getTexture())
		{
			m_sprite.setTexture(*m_defaultTexture);
		}
		break;
	case kHovered:
		m_hoveredTexture = texture;
		break;
	case kPressed: 
		m_pressedTexture = texture;
		break;
	}
}

void Button::setEnabled(bool enabled)
{
	if (enabled == m_enabled)
	{
		return;
	}

	m_enabled = enabled;
	m_sprite.setColor({ 255, 255, 255, m_enabled ? (sf::Uint8)255 : (sf::Uint8)128 });
	changeState(kDefault);
}

bool Button::update(const sf::Event& event, const sf::Vector2f& mousePositionInWindow)
{
	if (!m_enabled)
	{
		return false;
	}

	bool isButtonClicked = false;

	// Default state can transition to hovered if mouse position is over the button
	// Default state can transition to pressed if mouse position is over the button AND left mouse button is pressed
	if (m_state == kDefault)
	{
		if (m_rect.contains(mousePositionInWindow))
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
		if (m_rect.contains(mousePositionInWindow))
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
		if (m_rect.contains(mousePositionInWindow))
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
		m_sprite.setTexture(*m_defaultTexture);
		break;
	case kHovered:
		m_sprite.setTexture(*m_hoveredTexture);
		break;
	case kPressed:
		m_sprite.setTexture(*m_pressedTexture);
		break;
	}
}