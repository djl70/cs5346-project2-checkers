#pragma once

#include "checker_piece.h"

#include <SFML/Graphics.hpp>

class Checkerboard;
class Command;
class ResourceManager;

class Player
{
public:
	Player(CheckerColor color);

	void setBoard(Checkerboard* pBoard);
	void setResources(ResourceManager* pResources);

	virtual void takeTurn();
	bool isTurn() const;

	virtual void event(const sf::Event& event) = 0;
	virtual Command* update() = 0;
	virtual void render(sf::RenderWindow* pWindow) = 0;

protected:
	bool m_isTurn;
	CheckerColor m_color;
	Checkerboard* m_pBoard;
	ResourceManager* m_pResources;
};