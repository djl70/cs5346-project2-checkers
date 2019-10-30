#pragma once

#include "button.h"

class ToggleButton : public Button
{
public:
	ToggleButton();

	void toggleOn();
	void toggleOff();

	bool update(const sf::Event& event, const sf::Vector2f& mousePositionInWindow) override;

private:
	bool m_isToggledOn;
};