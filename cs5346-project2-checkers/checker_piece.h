#pragma once

#include <SFML/Graphics.hpp>

enum CheckerColor
{
	kBlack,
	kWhite
};

enum CheckerRank
{
	kMan,
	kKing
};


class CheckerPiece
{
public:
	CheckerPiece(float radius, CheckerColor color)
	{
		m_rank = kMan;
		m_color = color;
		m_shape = sf::CircleShape(radius);
		m_shape.setFillColor(getColorFromType(color));
		m_shape.setOrigin(radius, radius);
		m_isAttachedToMouse = false;
	}

	void makeKing()
	{
		m_rank = kKing;
	}

	CheckerRank getRank() const
	{
		return m_rank;
	}

	CheckerColor getColor() const
	{
		return m_color;
	}

	void setPosition(const sf::Vector2f& position)
	{
		m_shape.setPosition(position);
	}

	void attachToMouse()
	{
		m_isAttachedToMouse = true;
	}

	void detachFromMouse(const sf::Vector2f& placeAt)
	{
		m_isAttachedToMouse = false;
		m_shape.setPosition(placeAt);
	}

	bool isAttachedToMouse() const
	{
		return m_isAttachedToMouse;
	}

	void update(sf::RenderWindow* pWindow, const sf::Time& deltaTime)
	{
		if (m_isAttachedToMouse)
		{
			sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*pWindow));
			m_shape.setPosition(mousePosition);
		}
	}

	void draw(sf::RenderWindow* pWindow) const
	{
		pWindow->draw(m_shape);

		if (m_rank == kKing)
		{
			pWindow->draw(createCrownShape());
		}
	}

private:
	CheckerRank m_rank;
	CheckerColor m_color;
	bool m_isAttachedToMouse;
	sf::CircleShape m_shape;

	sf::ConvexShape createCrownShape() const
	{
		sf::ConvexShape crown;
		crown.setPointCount(7);

		sf::Vector2f center = m_shape.getPosition();
		float radius = m_shape.getRadius();

		crown.setPoint(0, center - sf::Vector2f(-0.5f * radius, -0.25f * radius));
		crown.setPoint(1, center - sf::Vector2f(-0.5f * radius, 0.25f * radius));
		crown.setPoint(2, center - sf::Vector2f(-0.25f * radius, 0.125f * radius));
		crown.setPoint(3, center - sf::Vector2f(0.0f * radius, 0.25f * radius));
		crown.setPoint(4, center - sf::Vector2f(0.25f * radius, 0.125f * radius));
		crown.setPoint(5, center - sf::Vector2f(0.5f * radius, 0.25f * radius));
		crown.setPoint(6, center - sf::Vector2f(0.5f * radius, -0.25f * radius));

		crown.setFillColor(sf::Color::Red);

		return crown;
	}

	sf::Color getColorFromType(CheckerColor color) const
	{
		switch (color)
		{
		case kBlack:
			return sf::Color(107, 73, 63, 255);
		case kWhite:
			return sf::Color(244, 200, 69, 255);
		default:
			return sf::Color::Black;
		}
	}
};