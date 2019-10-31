#pragma once

#include "checkerboard.h"

#include <SFML/Graphics.hpp>

class FullMoveCommand;
class ResourceManager;

class Player
{
public:
	Player(CheckerColor color, bool isBot);

	void setBoard(checkerboard::Checkerboard* pBoard);
	void setResources(ResourceManager* pResources);

	bool isBot() const;
	bool isTurn() const;
	CheckerColor getColor() const;

	virtual void takeTurn();
	virtual void event(const sf::Event& event) = 0;
	virtual FullMoveCommand* update() = 0;
	virtual void render(sf::RenderWindow* pWindow) = 0;

protected:
	CheckerColor m_color;
	bool m_isBot;
	bool m_isTurn;
	checkerboard::Checkerboard* m_pBoard;
	ResourceManager* m_pResources;
};