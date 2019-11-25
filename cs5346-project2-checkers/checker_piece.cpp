#include "checker_piece.h"

#include "config.h"

CheckerPiece::CheckerPiece()
	: m_rank{ kMan }
{
	m_sprite.setScale({ config::kScaling, config::kScaling });
}

void CheckerPiece::setPosition(const sf::Vector2f& position)
{
	m_position = position;
	m_sprite.setPosition(position);
}

void CheckerPiece::setColor(CheckerColor color)
{
	m_color = color;
}

void CheckerPiece::setTextures(sf::Texture* manTexture, sf::Texture* kingTexture)
{
	m_manTexture = manTexture;
	m_kingTexture = kingTexture;
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

bool CheckerPiece::isKing() const
{
	return m_rank == kKing;
}

CheckerColor CheckerPiece::getColor() const
{
	return m_color;
}

void CheckerPiece::render(sf::RenderWindow* pWindow) const
{
	pWindow->draw(m_sprite);
}

void CheckerPiece::updateTexture()
{
	switch (m_rank)
	{
	case kMan:
		m_sprite.setTexture(*m_manTexture);
		break;
	case kKing:
		m_sprite.setTexture(*m_kingTexture);
		break;
	}
}