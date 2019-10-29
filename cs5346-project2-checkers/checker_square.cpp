#include "checker_square.h"

#include "config.h"

CheckerSquare::CheckerSquare(CheckerSquareType type)
	: m_type{ type }
	, m_interactable{ type == kBlackSquare }
	, m_piece{ nullptr }
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
}

void CheckerSquare::setPromotionColor(CheckerColor color)
{
	m_promoteColor = color;
	m_isKingRow = true;
}

void CheckerSquare::setPosition(const sf::Vector2f& position)
{
	m_rect = { position.x, position.y, config::kSquareWidth, config::kSquareWidth };
	m_shape.setPosition({ m_rect.left, m_rect.top });
	m_shape.setSize({ m_rect.width, m_rect.height });
}

void CheckerSquare::setPiece(CheckerPiece* piece)
{
	m_piece = piece;
	if (m_piece)
	{
		m_piece->setPosition({ m_rect.left, m_rect.top });
		if (m_isKingRow && m_promoteColor == m_piece->getColor())
		{
			m_piece->promote();
		}
	}
}

void CheckerSquare::setNeighbors(const NeighboringSquares& neighbors)
{
	m_neighbors = neighbors;
}

NeighboringSquares CheckerSquare::getNeighbors() const
{
	return m_neighbors;
}

bool CheckerSquare::isEmpty() const
{
	return m_piece == nullptr;
}

CheckerPiece* CheckerSquare::getPiece()
{
	return m_piece;
}

bool CheckerSquare::contains(const sf::Vector2f& point) const
{
	return m_interactable && m_rect.contains(point);
}

void CheckerSquare::render(sf::RenderWindow* pWindow, bool highlight)
{
	if (highlight)
	{
		m_shape.setOutlineThickness(config::kSquareWidth * 0.05f);
		m_shape.setOutlineColor(sf::Color::White);
	}
	else
	{
		m_shape.setOutlineThickness(0.0f);
	}
	pWindow->draw(m_shape);
	if (m_piece)
	{
		m_piece->render(pWindow);
	}
}