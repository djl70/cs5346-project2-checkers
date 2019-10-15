#pragma once

#include "checkerboard.h"

class CheckersGameEngine
{
public:
	CheckersGameEngine(sf::RenderWindow* pWindow)
	{
		m_pWindow = pWindow;
	}

	void run()
	{
		mainLoop();
	}

private:
	sf::RenderWindow* m_pWindow;
	Checkerboard m_board;

	void mainLoop()
	{
		sf::Vector2i mouseDragStart;
		sf::Vector2i mouseDragEnd;
		bool isMouseDragging = false;

		sf::Clock clock;

		while (m_pWindow->isOpen())
		{
			sf::Time deltaTime = clock.restart();

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

			update(deltaTime);

			m_pWindow->clear();
			render();
			m_pWindow->display();
		}
	}

	void update(const sf::Time& deltaTime)
	{
		m_board.update(m_pWindow, deltaTime);
	}

	void render() const
	{
		m_board.draw(m_pWindow);
	}
};