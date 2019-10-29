#pragma once

#include <SFML/Graphics.hpp>

enum CheckerColor
{
	kBlack,
	kRed
};

enum CheckerRank
{
	kMan,
	kKing
};

class CheckerPiece
{
public:
	CheckerPiece(CheckerColor color, const sf::Texture& manTexture, const sf::Texture& kingTexture);

	void promote();
	void setPosition(const sf::Vector2f& position);

	bool isKing() const;
	CheckerColor getColor() const;
	bool contains(const sf::Vector2f& point) const;

	void render(sf::RenderWindow* pWindow) const;

private:
	CheckerColor m_color;
	CheckerRank m_rank;
	sf::Vector2f m_position;
	const sf::Texture& m_manTexture;
	const sf::Texture& m_kingTexture;
	sf::Sprite m_sprite;

	void updateTexture();
};