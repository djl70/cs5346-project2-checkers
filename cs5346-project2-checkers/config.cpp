#include "config.h"

namespace config
{
	const int kNumMovesWithoutCaptureOrKingingNeededToDraw = 50;
	const int kBoardStateFrequencyNeededToDraw = 3;

	const unsigned int kFps = 60;
	const float kScaling = 1.0f; // Controls the scaling of everything. For values other than 1, the graphics will likely be blurry.
	
	const float kScreenWidth = 1920.0f * kScaling;
	const float kScreenHeight = 1080.0f * kScaling;
	
	const float kSquareWidth = 100.0f * kScaling;
	
	const float kCapturedAreaOffsetFromEdge = 200.0f * kScaling;
	
	const sf::Vector2f boardTopLeft{ config::kScreenWidth * 0.5f - (config::kSquareWidth * 4.0f), config::kScreenHeight * 0.5f - (config::kSquareWidth * 4.0f) };
	const sf::Vector2f capturedBlackTopLeft{ kCapturedAreaOffsetFromEdge, config::kScreenHeight * 0.5f - (config::kSquareWidth * 2.0f) };
	const sf::Vector2f capturedRedTopLeft{ kScreenWidth - config::kSquareWidth * 3.0f - kCapturedAreaOffsetFromEdge, config::kScreenHeight * 0.5f - (config::kSquareWidth * 2.0f) };
	
	const sf::Color blackSquareColor{ 143, 67, 15, 200 };
	const sf::Color redSquareColor{ 242, 174, 126, 200 };

	const sf::Vector2f redTurnIndicatorTopLeft{ capturedRedTopLeft.x + kSquareWidth, capturedRedTopLeft.y - kSquareWidth * 1.5f };
	const sf::Vector2f blackTurnIndicatorTopLeft{ capturedBlackTopLeft.x + kSquareWidth, capturedBlackTopLeft.y - kSquareWidth * 1.5f };
	
	const sf::Vector2f titleSize{ 600.0f * kScaling, 100.0f * kScaling };
	const sf::FloatRect titleRect{ kScreenWidth * 0.5f - (titleSize.x * 0.5f), kScreenHeight * 0.25f - (titleSize.y * 0.5f), titleSize.x, titleSize.y };

	const sf::Vector2f menuButtonSize{ 500.0f * kScaling, 100.0f * kScaling };
	const sf::FloatRect multiPlayerButtonRect{ kScreenWidth * 0.5f - (menuButtonSize.x * 0.5f), kScreenHeight * 0.4f - (menuButtonSize.y * 0.5f), menuButtonSize.x, menuButtonSize.y };
	const sf::FloatRect singlePlayerButtonRect{ kScreenWidth * 0.5f - (menuButtonSize.x * 0.5f), kScreenHeight * 0.5f - (menuButtonSize.y * 0.5f), menuButtonSize.x, menuButtonSize.y };
	const sf::FloatRect autoPlayButtonRect{     kScreenWidth * 0.5f - (menuButtonSize.x * 0.5f), kScreenHeight * 0.6f - (menuButtonSize.y * 0.5f), menuButtonSize.x, menuButtonSize.y };

	const sf::FloatRect startButtonRect{ kScreenWidth * 0.5f - (menuButtonSize.x * 0.5f), kScreenHeight * 0.7f - (menuButtonSize.y * 0.5f), menuButtonSize.x, menuButtonSize.y };
	const sf::FloatRect menuButtonRect{ kScreenWidth * 0.5f - (menuButtonSize.x * 0.5f), kScreenHeight * 0.8f - (menuButtonSize.y * 0.5f), menuButtonSize.x, menuButtonSize.y };

	const sf::Vector2f leftAlgorithmButtonGroupPosition{ kScreenWidth * 0.3f - (menuButtonSize.x * 0.5f), kScreenHeight * 0.2f - (menuButtonSize.y * 0.5f) };
	const sf::Vector2f leftHeuristicButtonGroupPosition{ leftAlgorithmButtonGroupPosition.x,              kScreenHeight * 0.45f - (menuButtonSize.y * 0.5f) };

	const sf::Vector2f rightAlgorithmButtonGroupPosition{ kScreenWidth * 0.7f - (menuButtonSize.x * 0.5f), leftAlgorithmButtonGroupPosition.y };
	const sf::Vector2f rightHeuristicButtonGroupPosition{ rightAlgorithmButtonGroupPosition.x,             leftHeuristicButtonGroupPosition.y };

	const int kMaxSearchDepth = 4;

	const std::unordered_map<std::string, std::string> textureFiles = {
		{ "plain_background", "res/background.png" },
		{ "background", "res/checkers_game_field.png" },
		{ "red_king", "res/checker_piece_cream_king.png" },
		{ "black_king", "res/checker_piece_brown_king.png" },
		{ "red_man", "res/checker_piece_cream.png" },
		{ "black_man", "res/checker_piece_brown.png" },
		{ "title", "res/title.png" },
		{ "button_autoplay", "res/button_autoplay.png" },
		{ "button_autoplay_hover", "res/button_autoplay_hover.png" },
		{ "button_autoplay_press", "res/button_autoplay_press.png" },
		{ "button_multiplayer", "res/button_multiplayer.png" },
		{ "button_multiplayer_hover", "res/button_multiplayer_hover.png" },
		{ "button_multiplayer_press", "res/button_multiplayer_press.png" },
		{ "button_singleplayer", "res/button_singleplayer.png" },
		{ "button_singleplayer_hover", "res/button_singleplayer_hover.png" },
		{ "button_singleplayer_press", "res/button_singleplayer_press.png" },
		{ "button_minimax", "res/button_minimax.png" },
		{ "button_minimax_hover", "res/button_minimax_hover.png" },
		{ "button_minimax_press", "res/button_minimax_press.png" },
		{ "button_alphabeta", "res/button_alphabeta.png" },
		{ "button_alphabeta_hover", "res/button_alphabeta_hover.png" },
		{ "button_alphabeta_press", "res/button_alphabeta_press.png" },
		{ "button_h1", "res/button_h1.png" },
		{ "button_h1_hover", "res/button_h1_hover.png" },
		{ "button_h1_press", "res/button_h1_press.png" },
		{ "button_h2", "res/button_h2.png" },
		{ "button_h2_hover", "res/button_h2_hover.png" },
		{ "button_h2_press", "res/button_h2_press.png" },
		{ "button_start", "res/button_start.png" },
		{ "button_start_hover", "res/button_start_hover.png" },
		{ "button_start_press", "res/button_start_press.png" },
		{ "button_menu", "res/button_menu.png" },
		{ "button_menu_hover", "res/button_menu_hover.png" },
		{ "button_menu_press", "res/button_menu_press.png" },
		{ "winner", "res/winner.png" }
	};

	const std::unordered_map<std::string, std::string> fontFiles = {
		{ "font_arial", "res/arial.ttf" }
	};

	const std::unordered_map<std::string, std::string> soundFiles = {
		{ "sound_move", "res/snap.wav" },
		{ "sound_jump", "res/pop_meow.wav" }
	};
}