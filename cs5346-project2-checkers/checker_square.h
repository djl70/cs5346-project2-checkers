#pragma once

#include <SFML/Graphics.hpp>

class CheckerPiece;

class CheckerSquare
{
public:
	CheckerSquare(const sf::FloatRect& rect);

	sf::FloatRect getRect() const;

	void placePiece(CheckerPiece* piece);
	CheckerPiece* getPiece() const;

private:
	sf::FloatRect m_rect;
	CheckerPiece* m_piece;
};

void movePiece(CheckerSquare& from, CheckerSquare& to);



//bool isEmpty(const CheckerSquare& square)
//{
//	return square.getPiece() == nullptr;
//}

/*
#include "checker_piece.h"

#include <stdexcept>

class CheckerSquare
{
public:
	CheckerSquare()
	{
		m_piece = nullptr;
	}

	CheckerSquare(const sf::FloatRect& rect, CheckerColor color)
	{
		m_color = color;
		m_squareShape = sf::RectangleShape({ rect.width, rect.height });
		m_squareShape.setPosition({ rect.left + rect.width * 0.5f, rect.top + rect.height * 0.5f });
		m_squareShape.setFillColor(getColorFromType(color));
		m_squareShape.setOrigin(rect.width * 0.5f, rect.height * 0.5f);
		m_piece = nullptr;
	}

	bool canPlacePiece() const
	{
		return !hasPiece() && m_color == kBlack;
	}

	CheckerRank getPieceRank() const
	{
		if (!hasPiece())
		{
			throw std::runtime_error("No piece to get the rank of");
		}

		return m_piece->getRank();
	}

	CheckerColor getPieceColor() const
	{
		if (!hasPiece())
		{
			throw std::runtime_error("No piece to get the color of");
		}

		return m_piece->getColor();
	}

	void kingPiece()
	{
		if (!hasPiece())
		{
			throw std::runtime_error("No piece to king");
		}

		m_piece->makeKing();
	}

	bool hasPiece() const
	{
		return m_piece != nullptr;
	}

	void placePiece(CheckerPiece* piece)
	{
		if (hasPiece())
		{
			throw std::runtime_error("Cannot place a piece on an occupied square");
		}

		m_piece = piece;
		m_piece->setPosition(m_squareShape.getPosition());
	}

	void attachPieceToMouse()
	{
		if (!hasPiece())
		{
			throw std::runtime_error("No piece to attach to the mouse");
		}

		m_piece->attachToMouse();
	}

	void detachPieceFromMouse()
	{
		if (!hasPiece())
		{
			throw std::runtime_error("No piece to detach from the mouse");
		}

		m_piece->detachFromMouse(m_squareShape.getPosition());
	}

	bool isPieceAttachedToMouse() const
	{
		return hasPiece() && m_piece->isAttachedToMouse();
	}

	CheckerPiece* takePiece()
	{
		if (!hasPiece())
		{
			throw std::runtime_error("Cannot remove a piece from an unoccupied square");
		}

		CheckerPiece* piece = m_piece;
		m_piece = nullptr;
		return piece;
	}

	void update(sf::RenderWindow* pWindow, const sf::Time& deltaTime)
	{
		if (hasPiece())
		{
			m_piece->update(pWindow, deltaTime);
		}
	}

	void draw(sf::RenderWindow* pWindow) const
	{
		pWindow->draw(m_squareShape);
	}

	void drawPiece(sf::RenderWindow* pWindow) const
	{
		if (hasPiece())
		{
			m_piece->draw(pWindow);
		}
	}

private:
	CheckerColor m_color;
	sf::RectangleShape m_squareShape;
	CheckerPiece* m_piece;

	sf::Color getColorFromType(CheckerColor color) const
	{
		switch (color)
		{
		case kBlack:
			return sf::Color(166, 125, 93, 255);
		case kWhite:
			return sf::Color(232, 208, 170, 255);
		default:
			return sf::Color::Black;
		}
	}
};
*/