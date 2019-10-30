#include "checker_piece.h"

#include "config.h"

CheckerPiece::CheckerPiece(CheckerColor color, const sf::Texture& manTexture, const sf::Texture& kingTexture)
	: m_color{ color }
	, m_rank{ kMan }
	, m_manTexture{ manTexture }
	, m_kingTexture{ kingTexture }
{
	m_sprite.setScale({ config::kScaling, config::kScaling });
	updateTexture();
}

void CheckerPiece::demote()
{
	m_rank = kMan;
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

bool CheckerPiece::isKing() const
{
	return m_rank == kKing;
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
	/*switch (m_color)
	{
	case kBlack:
		if (m_rank == kMan) { m_sprite.setTexture(resources::pResources->getTexture("black_man")); }
		else if (m_rank == kKing) { m_sprite.setTexture(resources::pResources->getTexture("black_king")); }
		break;
	case kRed:
		if (m_rank == kMan) { m_sprite.setTexture(resources::pResources->getTexture("red_man")); }
		else if (m_rank == kKing) { m_sprite.setTexture(resources::pResources->getTexture("red_king")); }
		break;
	}*/
	switch (m_rank)
	{
	case kMan:
		m_sprite.setTexture(m_manTexture);
		break;
	case kKing:
		m_sprite.setTexture(m_kingTexture);
		break;
	}
}