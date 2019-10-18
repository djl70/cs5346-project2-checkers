#include "checkers_game_engine.h"

#include <SFML/Graphics.hpp>

int main()
{
	const unsigned int fps = 60;

	sf::RenderWindow window(sf::VideoMode(800, 800), "Checkers", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(fps);
	
	CheckersGameEngine gameEngine(&window, fps);
	gameEngine.run();

	return 0;
}