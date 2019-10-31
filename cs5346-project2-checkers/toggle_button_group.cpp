#include "toggle_button_group.h"

ToggleButtonGroup::ToggleButtonGroup()
	: m_activeButton{ 0 }
{

}

void ToggleButtonGroup::addButton(const ToggleButton& button)
{
	m_buttons.push_back(button);
	if (m_buttons.size() == 1)
	{
		// This was the first button, so toggle it on
		m_buttons.back().toggleOn();
	}
	else
	{
		m_buttons.back().toggleOff();
	}
}

int ToggleButtonGroup::getActiveButtonIndex() const
{
	return m_activeButton;
}

void ToggleButtonGroup::setEnabled(bool enabled)
{
	for (auto& button : m_buttons)
	{
		button.setEnabled(enabled);
	}
}

bool ToggleButtonGroup::update(const sf::Event& event, const sf::Vector2f& mousePositionInWindow)
{
	bool activeButtonChanged = false;

	for (int i = 0; i < m_buttons.size(); ++i)
	{
		if (m_buttons.at(i).update(event, mousePositionInWindow))
		{
			// The button was toggled on
			m_activeButton = i;
			activeButtonChanged = true;
			break;
		}
	}

	// If we selected a different button, toggle-off the rest
	if (activeButtonChanged)
	{
		for (int i = 0; i < m_buttons.size(); ++i)
		{
			if (i != m_activeButton)
			{
				m_buttons.at(i).toggleOff();
			}
		}
	}

	return activeButtonChanged;
}
void ToggleButtonGroup::render(sf::RenderWindow* pWindow) const
{
	for (const auto& button : m_buttons)
	{
		button.render(pWindow);
	}
}