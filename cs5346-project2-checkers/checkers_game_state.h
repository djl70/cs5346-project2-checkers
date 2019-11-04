#pragma once

#include "base_state.h"
#include "checker_piece.h"
#include "checker_square.h"
#include "checkerboard.h"
#include "resource_manager.h"
#include "game_over_state.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <array>
#include <bitset>
#include <stack>
#include <unordered_map>
#include <vector>

class Command;
class Player;

class CheckersGameState : public BaseState
{
public:
	CheckersGameState(ResourceManager* pResources, Player* firstPlayer, Player* secondPlayer);

	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	ResourceManager* m_pResources;
	sf::Sprite m_background;
	sf::Sprite m_sprite;
	sf::Color m_blackPieceColor;
	sf::Color m_redPieceColor;
	sf::RectangleShape m_rect;
	checkerboard::Checkerboard m_board;
	//int m_numMovesSinceCaptureOrKinging;
	//std::unordered_map<std::bitset<checkerboard::kBitsToEncodeBoardState>, int> m_boardStateFrequency;

	//int m_currentPlayer;
	std::array<Player*, 2> m_players;
	std::array<CheckerPiece, 2> m_turnIndicators;
	std::array<CheckerSquare, 2> m_turnIndicatorSquares;

	std::stack<Command*> m_commands;

	bool isGameOver(GameOverCondition& outGameOverCondition);
};