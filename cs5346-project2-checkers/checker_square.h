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

class CheckerSquare;

struct NeighboringSquares
{
	CheckerSquare* pNeighborNorthWest = nullptr;
	CheckerSquare* pNeighborNorthEast = nullptr;
	CheckerSquare* pNeighborSouthWest = nullptr;
	CheckerSquare* pNeighborSouthEast = nullptr;
};

class CheckerSquare
{
public:
	CheckerSquare(CheckerSquareType type);

	void setPromotionColor(CheckerColor color);
	void setPosition(const sf::Vector2f& position);
	void setPiece(CheckerPiece* piece);

	void setNeighbors(const NeighboringSquares& neighbors);
	NeighboringSquares getNeighbors() const;

	bool isEmpty() const;
	CheckerPiece* getPiece();
	bool contains(const sf::Vector2f& point) const;

	void render(sf::RenderWindow* pWindow, bool highlight);// const;

private:
	CheckerSquareType m_type;
	sf::FloatRect m_rect;
	bool m_interactable;
	CheckerPiece* m_piece;
	bool m_isKingRow;
	CheckerColor m_promoteColor;

	NeighboringSquares m_neighbors;

	sf::RectangleShape m_shape;
};