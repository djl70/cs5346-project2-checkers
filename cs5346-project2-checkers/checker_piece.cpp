#include "checker_piece.h"

#include "config.h"
#include "resources.h"

CheckerPiece::CheckerPiece(CheckerColor color)
	: m_color{ color }
	, m_rank{ kMan }
{
	m_sprite.setScale({ config::kScaling, config::kScaling });
	updateTexture();
}

void CheckerPiece::promote()
{
	m_rank = kKing;
	updateTexture();
}

void CheckerPiece::setPosition(const sf::Vector2f& position)
{
	m_position = position;
	m_sprite.setPosition(position);
}

CheckerColor CheckerPiece::getColor() const
{
	return m_color;
}

bool CheckerPiece::contains(const sf::Vector2f& point) const
{
	sf::FloatRect rect({ m_position.x, m_position.y, config::kSquareWidth, config::kSquareWidth });
	return rect.contains(point);
}

void CheckerPiece::render(sf::RenderWindow* pWindow) const
{
	pWindow->draw(m_sprite);
}

void CheckerPiece::updateTexture()
{
	switch (m_color)
	{
	case kBlack:
		if (m_rank == kMan) { m_sprite.setTexture(resources::textures["black_man"]); }
		else if (m_rank == kKing) { m_sprite.setTexture(resources::textures["black_king"]); }
		break;
	case kRed:
		if (m_rank == kMan) { m_sprite.setTexture(resources::textures["red_man"]); }
		else if (m_rank == kKing) { m_sprite.setTexture(resources::textures["red_king"]); }
		break;
	}
}