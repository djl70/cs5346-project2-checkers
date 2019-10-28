#pragma once

#include <SFML/Graphics.hpp>

class BaseState;

class CheckersGameEngine
{
public:
	CheckersGameEngine(unsigned int fps);
	~CheckersGameEngine();

	void run();

private:
	sf::RenderWindow m_window;
	BaseState* m_pState;
};