#pragma once

#include "toggle_button.h"

#include <vector>

class ToggleButtonGroup
{
public:
	ToggleButtonGroup();

	void addButton(const ToggleButton& button);
	int getActiveButtonIndex() const;

	void setEnabled(bool enabled);

	bool update(const sf::Event& event, const sf::Vector2f& mousePositionInWindow);
	void render(sf::RenderWindow* pWindow) const;

private:
	std::vector<ToggleButton> m_buttons;
	int m_activeButton;
};