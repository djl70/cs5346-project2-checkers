#pragma once

#include "base_state.h"
#include "button.h"
#include "resource_manager.h"

#include <SFML/Graphics.hpp>

class MainMenuState : public BaseState
{
public:
	MainMenuState(ResourceManager& resources);

	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	ResourceManager& m_resources;
	sf::Sprite m_background;
	sf::Sprite m_title;
	Button m_singlePlayerButton;
	Button m_multiPlayerButton;
	Button m_autoPlayButton;
};