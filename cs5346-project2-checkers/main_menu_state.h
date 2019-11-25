#pragma once

#include "base_state.h"
#include "button.h"
#include "resource_manager.h"

#include <SFML/Graphics.hpp>

class MainMenuState : public BaseState
{
public:
	MainMenuState(ResourceManager* pResources);
	~MainMenuState();

	void enter() override;
	BaseState* event() override;
	void render() override;

private:
	ResourceManager* m_pResources;
	sf::Sprite m_background;
	sf::Sprite m_title;
	Button m_multiPlayerButton;
	Button m_singlePlayerButton;
	Button m_autoPlayButton;
};