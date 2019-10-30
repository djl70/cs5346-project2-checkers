#include "toggle_button.h"

ToggleButton::ToggleButton()
	: Button()
	, m_isToggledOn{ false }
{

}

void ToggleButton::toggleOn()
{
	m_isToggledOn = true;
	changeState(kPressed);
}

void ToggleButton::toggleOff()
{
	m_isToggledOn = false;
	changeState(kDefault);
}

bool ToggleButton::update(const sf::Event& event, const sf::Vector2f& mousePositionInWindow)
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
	// While toggled on, pressed state cannot be exited except via an external call to toggleOff()
	// While toggled off, pressed state can transition to default if the mouse leaves the button
	else if (m_state == kPressed && !m_isToggledOn)
	{
		if (m_rect.contains(mousePositionInWindow))
		{
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				// Button is clicked when mouse is released while button is in pressed state
				isButtonClicked = true;
				toggleOn();
			}
		}
		else
		{
			changeState(kDefault);
		}
	}

	return isButtonClicked;
}