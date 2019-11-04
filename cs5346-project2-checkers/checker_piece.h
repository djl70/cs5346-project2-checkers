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
	CheckerPiece();

	void setPosition(const sf::Vector2f& position);
	void setColor(CheckerColor color);
	void setTextures(sf::Texture* manTexture, sf::Texture* kingTexture);
	void demote();
	void promote();

	bool isKing() const;
	CheckerColor getColor() const;
	//bool contains(const sf::Vector2f& point) const;

	void render(sf::RenderWindow* pWindow) const;

private:
	CheckerColor m_color;
	CheckerRank m_rank;
	sf::Vector2f m_position;
	sf::Texture* m_manTexture;
	sf::Texture* m_kingTexture;
	sf::Sprite m_sprite;

	void updateTexture();
};