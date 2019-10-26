#include "checker_square.h"

#include "checker_piece.h"

CheckerSquare::CheckerSquare(const sf::FloatRect& rect)
	: m_rect{ rect }
	, m_piece{ nullptr }
{

}

sf::FloatRect CheckerSquare::getRect() const
{
	return m_rect;
}

void CheckerSquare::placePiece(CheckerPiece* piece)
{
	m_piece = piece;
}

CheckerPiece* CheckerSquare::getPiece() const
{
	return m_piece;
}

void movePiece(CheckerSquare& from, CheckerSquare& to)
{
	CheckerPiece* piece = from.getPiece();
	from.placePiece(nullptr);
	to.placePiece(piece);
}