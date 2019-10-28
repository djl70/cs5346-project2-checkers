#pragma once

#include "base_state.h"
#include "checker_piece.h"
#include "checker_square.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class CheckersGameState : public BaseState
{
public:
	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	sf::Sound m_moveSound;
	sf::Sound m_jumpSound;
	sf::Sprite m_background;
	sf::Sprite m_sprite;
	sf::Color m_blackPieceColor;
	sf::Color m_redPieceColor;
	sf::RectangleShape m_rect;
	std::vector<CheckerPiece> m_pieces;
	std::vector<CheckerSquare> m_squares;
	std::vector<CheckerSquare> m_capturedRedSquares;
	std::vector<CheckerSquare> m_capturedBlackSquares;

	bool isGameOver(CheckerColor& outWinningColor) const;

	enum CheckerSelectionProgress
	{
		// Player's turn just started, no piece is selected yet
		kNoCheckerSelected,

		// Player started holding down the left mouse button while hovering over a piece
		kCheckerGrabbed,

		// Player stopped holding down the left mouse button while a piece was grabbed
		kCheckerReleased,

		// Player grabbed and released a piece already, but the move was not valid
		kCheckerSelected
	};
	CheckerSelectionProgress m_selectionProgress;
	int m_selectedPiece;
};