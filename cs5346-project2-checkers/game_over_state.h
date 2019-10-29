#pragma once

#include "base_state.h"
#include "checker_piece.h"

#include <SFML/Graphics.hpp>

class GameOverState : public BaseState
{
public:
	GameOverState(CheckerColor winningColor);

	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	CheckerPiece m_piece;
	sf::Sprite m_background;
};