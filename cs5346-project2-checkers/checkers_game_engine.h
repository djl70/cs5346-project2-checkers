#pragma once

#include "checkers_game_state.h"

#include <SFML/Graphics.hpp>

#include <stack>
#include <vector>

class Command;
class Gui;

class CheckersGameEngine
{
public:
	CheckersGameEngine(sf::RenderWindow* pWindow);
	~CheckersGameEngine();

	void performCommand(Command* command);
	void undoLastCommand();

	void run();

private:
	sf::RenderWindow* m_pWindow;
	sf::Sprite m_background;
	sf::FloatRect m_boardRect;
	std::stack<Command*> m_commandHistory;

	std::vector<CheckerPiece> m_pieces;
	std::vector<CheckerSquare> m_squares;
	CheckersGameState m_gameState;

	Gui* m_gui;

	bool loadResources();
	void applyTextures();
	void initializeGameState();

	void mainLoop();
	void processEvent(const sf::Event& event);
	void render();
};

/*
#include "checkerboard.h"

class CheckersGameEngine
{
public:
	CheckersGameEngine(sf::RenderWindow* pWindow, unsigned int fps)
	{
		m_pWindow = pWindow;
		m_fps = fps;
		m_timePerUpdate = sf::milliseconds(1000 / fps);
	}

	void run()
	{
		mainLoop();
	}

private:
	sf::RenderWindow* m_pWindow;
	Checkerboard m_board;
	sf::Vector2i mouseDragStart;
	sf::Vector2i mouseDragEnd;
	bool isMouseDragging = false;
	unsigned int m_fps;
	sf::Time m_timePerUpdate;

	void processEvents()
	{
		sf::Event event;
		while (m_pWindow->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				m_pWindow->close();
				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					mouseDragStart = { event.mouseButton.x, event.mouseButton.y };
					m_board.dragPiece(mouseDragStart.y / 100, mouseDragStart.x / 100);
					isMouseDragging = true;
				}
				break;

			case sf::Event::MouseButtonReleased:
				if (isMouseDragging && event.mouseButton.button == sf::Mouse::Button::Left)
				{
					mouseDragEnd = { event.mouseButton.x, event.mouseButton.y };
					m_board.movePiece(mouseDragStart.y / 100, mouseDragStart.x / 100, mouseDragEnd.y / 100, mouseDragEnd.x / 100);
					isMouseDragging = false;
				}
				break;
			}
		}
	}

	void mainLoop()
	{
		sf::Clock clock;
		sf::Time previousTime = clock.getElapsedTime();
		sf::Time lagTime;

		while (m_pWindow->isOpen())
		{
			sf::Time currentTime = clock.getElapsedTime();
			sf::Time deltaTime = currentTime - previousTime;
			previousTime = currentTime;
			lagTime += deltaTime;

			processEvents();

			while (lagTime >= m_timePerUpdate)
			{
				update(m_timePerUpdate);
				lagTime -= m_timePerUpdate;
			}

			render();
		}
	}

	void update(const sf::Time& deltaTime)
	{
		m_board.update(m_pWindow, deltaTime);
	}

	void render() const
	{
		m_pWindow->clear();

		m_board.draw(m_pWindow);

		m_pWindow->display();
	}
};
*/