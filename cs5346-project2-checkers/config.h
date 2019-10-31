#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

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
	extern const sf::Vector2f redTurnIndicatorTopLeft;
	extern const sf::Vector2f blackTurnIndicatorTopLeft;
	extern const sf::Vector2f titleSize;
	extern const sf::Vector2f menuButtonSize;
	extern const sf::FloatRect titleRect;
	extern const sf::FloatRect multiPlayerButtonRect;
	extern const sf::FloatRect singlePlayerButtonRect;
	extern const sf::FloatRect autoPlayButtonRect;
	extern const sf::FloatRect startButtonRect;
	extern const sf::FloatRect menuButtonRect;
	extern const sf::Vector2f leftAlgorithmButtonGroupPosition;
	extern const sf::Vector2f leftHeuristicButtonGroupPosition;
	extern const sf::Vector2f rightAlgorithmButtonGroupPosition;
	extern const sf::Vector2f rightHeuristicButtonGroupPosition;

	extern const std::unordered_map<std::string, std::string> textureFiles;
	extern const std::unordered_map<std::string, std::string> soundFiles;
}