#include "checker_square.h"

#include "config.h"

CheckerSquare::CheckerSquare(CheckerSquareType type)
	: m_type{ type }
	, m_interactable{ type == kBlackSquare }
	, m_piece{ -1 }
	, m_isKingRow{ false }
{
	sf::Color fillColor;
	switch (m_type)
	{
	case kBlackSquare:
		fillColor = config::blackSquareColor;
		break;
	case kRedSquare:
		fillColor = config::redSquareColor;
		break;
	default:
		fillColor = sf::Color::Transparent;
		break;
	}
	m_shape.setFillColor(fillColor);

	m_highlightShape.setFillColor(sf::Color::Transparent);
	m_highlightShape.setOutlineColor(sf::Color::White);
	m_highlightShape.setOutlineThickness(config::kSquareWidth * 0.05f);
}

void CheckerSquare::setPromotionColor(CheckerColor color)
{
	m_promoteColor = color;
	m_isKingRow = true;
}

void CheckerSquare::setPositionOnBoard(const sf::Vector2i& coords, const sf::Vector2f& referencePoint)
{
	m_positionOnBoard = coords;
	setPosition({
		referencePoint.x + coords.x * config::kSquareWidth,
		referencePoint.y + coords.y * config::kSquareWidth
	});
}

void CheckerSquare::setPieceIndex(int pieceIndex)
{
	m_piece = pieceIndex;
}

bool CheckerSquare::promotesColor(CheckerColor color) const
{
	return m_isKingRow && m_promoteColor == color;
}

sf::Vector2i CheckerSquare::getPositionOnBoard() const
{
	return m_positionOnBoard;
}

sf::Vector2f CheckerSquare::getPositionInWindow() const
{
	return { m_rect.left, m_rect.top };
}

bool CheckerSquare::isEmpty() const
{
	return m_piece == -1;
}

int CheckerSquare::getPieceIndex() const
{
	return m_piece;
}

bool CheckerSquare::contains(const sf::Vector2f& point) const
{
	return m_interactable && m_rect.contains(point);
}

void CheckerSquare::render(sf::RenderWindow* pWindow) const
{
	pWindow->draw(m_shape);
}

void CheckerSquare::renderHighlight(sf::RenderWindow* pWindow) const
{
	pWindow->draw(m_highlightShape);
}

void CheckerSquare::setPosition(const sf::Vector2f& position)
{
	m_rect = { position.x, position.y, config::kSquareWidth, config::kSquareWidth };
	m_shape.setPosition({ m_rect.left, m_rect.top });
	m_shape.setSize({ m_rect.width, m_rect.height });

	m_highlightShape.setPosition(m_shape.getPosition());
	m_highlightShape.setSize(m_shape.getSize());
}