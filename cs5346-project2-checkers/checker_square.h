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
	// TODO: Allow this to be set after instantiation
	CheckerSquare(CheckerSquareType type);

	void setPromotionColor(CheckerColor color);
	void setPositionOnBoard(const sf::Vector2i& coords, const sf::Vector2f& referencePoint);
	void setPieceIndex(int pieceIndex);

	//void setNeighbors(const NeighboringSquares& neighbors);
	//NeighboringSquares getNeighbors() const;

	bool promotesColor(CheckerColor color) const;
	sf::Vector2i getPositionOnBoard() const;
	sf::Vector2f getPositionInWindow() const;
	bool isEmpty() const;
	int getPieceIndex() const;
	bool contains(const sf::Vector2f& point) const;

	void render(sf::RenderWindow* pWindow) const;
	void renderHighlight(sf::RenderWindow* pWindow) const;

private:
	CheckerSquareType m_type;
	sf::FloatRect m_rect;
	sf::Vector2i m_positionOnBoard;
	bool m_interactable;
	int m_piece;
	bool m_isKingRow;
	CheckerColor m_promoteColor;

	//NeighboringSquares m_neighbors;

	sf::RectangleShape m_shape;
	sf::RectangleShape m_highlightShape;

	void setPosition(const sf::Vector2f& position);
};