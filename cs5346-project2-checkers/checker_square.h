#pragma once

#include "checker_piece.h"

#include <SFML/Graphics.hpp>

enum CheckerSquareType
{
	kBlackSquare,
	kRedSquare,
	kCapturedBlack,
	kCapturedRed
};

class CheckerSquare
{
public:
	CheckerSquare(CheckerSquareType type);

	void setPromotionColor(CheckerColor color);
	void setPosition(const sf::Vector2f& position);
	void setPiece(CheckerPiece* piece);

	CheckerPiece* getPiece();
	bool contains(const sf::Vector2f& point) const;

	void render(sf::RenderWindow* pWindow) const;

private:
	CheckerSquareType m_type;
	sf::FloatRect m_rect;
	bool m_interactable;
	CheckerPiece* m_piece;
	bool m_isKingRow;
	CheckerColor m_promoteColor;

	sf::RectangleShape m_shape;
};