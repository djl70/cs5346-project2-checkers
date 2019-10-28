#include "config.h"

namespace config
{
	const unsigned int kFps = 60;
	const float kScaling = 1.0f; // Controls the scaling of everything. For values other than 1, the graphics will likely be blurry.
	
	const float kScreenWidth = 1920.0f * kScaling;
	const float kScreenHeight = 1080.0f * kScaling;
	
	const float kSquareWidth = 100.0f * kScaling;
	
	const float kCapturedAreaOffsetFromEdge = 200.0f * kScaling;
	
	const sf::Vector2f boardTopLeft{ (config::kScreenWidth - config::kSquareWidth * 8.0f) * 0.5f, (config::kScreenHeight - config::kSquareWidth * 8.0f) * 0.5f };
	const sf::Vector2f capturedRedTopLeft{ kCapturedAreaOffsetFromEdge, (config::kScreenHeight - config::kSquareWidth * 4.0f) * 0.5f };
	const sf::Vector2f capturedBlackTopLeft{ kScreenWidth - config::kSquareWidth * 3.0f - kCapturedAreaOffsetFromEdge, (config::kScreenHeight - config::kSquareWidth * 4.0f) * 0.5f };
	
	const sf::Color blackSquareColor{ 143, 67, 15, 200 };
	const sf::Color redSquareColor{ 242, 174, 126, 200 };
	
	const sf::Vector2f titleSize{ 600.0f * kScaling, 100.0f * kScaling };
	const sf::FloatRect titleRect{ (kScreenWidth - titleSize.x) * 0.5f, (kScreenHeight * 0.5f - titleSize.y) * 0.5f, titleSize.x, titleSize.y };

	const sf::Vector2f menuButtonSize{ 500.0f * kScaling, 100.0f * kScaling };
	const sf::FloatRect multiPlayerButtonRect{ (kScreenWidth - menuButtonSize.x) * 0.5f, (kScreenHeight * 0.8f - menuButtonSize.y) * 0.5f, menuButtonSize.x, menuButtonSize.y };
	const sf::FloatRect singlePlayerButtonRect{ (kScreenWidth - menuButtonSize.x) * 0.5f, (kScreenHeight * 1.0f - menuButtonSize.y) * 0.5f, menuButtonSize.x, menuButtonSize.y };
	const sf::FloatRect autoPlayButtonRect{ (kScreenWidth - menuButtonSize.x) * 0.5f, (kScreenHeight * 1.2f - menuButtonSize.y) * 0.5f, menuButtonSize.x, menuButtonSize.y };
}