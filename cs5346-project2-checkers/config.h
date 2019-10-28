#pragma once

#include <SFML/Graphics.hpp>

namespace config
{
	extern const unsigned int kFps;
	extern const float kScaling; // Controls the scaling of everything. For values other than 1, the graphics will likely be blurry.
	extern const float kScreenWidth;
	extern const float kScreenHeight;
	extern const float kSquareWidth;
	extern const float kCapturedAreaOffsetFromEdge;
	extern const sf::Vector2f boardTopLeft;
	extern const sf::Color blackSquareColor;
	extern const sf::Color redSquareColor;
	extern const sf::Vector2f capturedRedTopLeft;
	extern const sf::Vector2f capturedBlackTopLeft;
	extern const sf::Vector2f titleSize;
	extern const sf::Vector2f menuButtonSize;
	extern const sf::FloatRect titleRect;
	extern const sf::FloatRect multiPlayerButtonRect;
	extern const sf::FloatRect singlePlayerButtonRect;
	extern const sf::FloatRect autoPlayButtonRect;
}