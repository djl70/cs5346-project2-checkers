#pragma once

#include "base_state.h"
#include "button.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class MainMenuState : public BaseState
{
public:
	MainMenuState();

	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	sf::Sound m_buttonClick;
	sf::Sprite m_background;
	sf::Sprite m_title;
	Button m_singlePlayerButton;
	Button m_multiPlayerButton;
	Button m_autoPlayButton;
};