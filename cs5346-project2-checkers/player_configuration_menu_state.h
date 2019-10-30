#pragma once

#include "base_state.h"
#include "resource_manager.h"
#include "toggle_button_group.h"


class PlayerConfigurationMenuState : public BaseState
{
public:
	PlayerConfigurationMenuState(ResourceManager* pResources, unsigned int aiPlayerCount);

	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	ResourceManager* m_pResources;
	unsigned int m_aiPlayerCount;
	sf::Sprite m_background;
	sf::Sprite m_title;
	ToggleButtonGroup m_algorithmButtons;
	ToggleButtonGroup m_heuristicButtons;
	Button m_startGameButton;
	Button m_mainMenuButton;
};