#pragma once

#include "base_state.h"
#include "button.h"
#include "checker_piece.h"
#include "resource_manager.h"

#include <SFML/Graphics.hpp>

class GameOverState : public BaseState
{
public:
	GameOverState(ResourceManager* pResources, CheckerColor winningColor);

	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	ResourceManager* m_pResources;
	CheckerPiece m_piece;
	sf::Sprite m_background;
	sf::Sprite m_winnerText;
	Button m_menuButton;
};