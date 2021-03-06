#pragma once

#include "base_state.h"
#include "button.h"
#include "checker_piece.h"
#include "resource_manager.h"

#include <SFML/Graphics.hpp>

enum GameOverCondition
{
	kBlackHasNoPiecesLeft,
	kRedHasNoPiecesLeft,
	kBlackCannotMove,
	kRedCannotMove,
	kTurnLimitReached,
	kBoardStateRepetitionLimitReached
};

class GameOverState : public BaseState
{
public:
	GameOverState(ResourceManager* pResources, GameOverCondition condition);
	~GameOverState();

	void enter() override;
	BaseState* event() override;
	void render() override;

private:
	ResourceManager* m_pResources;
	CheckerPiece m_blackPiece;
	CheckerPiece m_redPiece;
	sf::Sprite m_background;
	sf::Text m_mainText;
	sf::Text m_subText;
	Button m_menuButton;
};