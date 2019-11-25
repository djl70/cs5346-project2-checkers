#pragma once

#include "checkerboard.h"

#include <SFML/Graphics.hpp>

class FullMoveCommand;
class ResourceManager;

class Player
{
public:
	Player(CheckerColor color, bool isBot);
	virtual ~Player() {};

	void setBoard(checkerboard::Checkerboard* pBoard);
	void setResources(ResourceManager* pResources);

	bool isBot() const;
	CheckerColor getColor() const;

	virtual void startTurn() = 0;
	virtual void stop() = 0;
	virtual void event(const sf::Event& event) = 0;
	virtual FullMoveCommand* update() = 0;
	virtual void render(sf::RenderWindow* pWindow) = 0;

protected:
	CheckerColor m_color;
	bool m_isBot;
	checkerboard::Checkerboard* m_pBoard;
	ResourceManager* m_pResources;
};