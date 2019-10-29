#pragma once

#include "resource_manager.h"

#include <SFML/Graphics.hpp>

class BaseState;

class CheckersGameEngine
{
public:
	CheckersGameEngine();
	~CheckersGameEngine();

	void run();

private:
	sf::RenderWindow m_window;
	BaseState* m_pState;
	ResourceManager m_resources;
};