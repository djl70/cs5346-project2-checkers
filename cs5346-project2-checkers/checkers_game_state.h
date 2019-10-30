#pragma once

#include "base_state.h"
#include "checker_piece.h"
#include "checker_square.h"
#include "checkerboard.h"
#include "resource_manager.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <array>
#include <stack>
#include <vector>

class Command;
class Player;

class CheckersGameState : public BaseState
{
public:
	CheckersGameState(ResourceManager& resources);

	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	ResourceManager& m_resources;
	sf::Sound m_moveSound;
	sf::Sound m_jumpSound;
	sf::Sprite m_background;
	sf::Sprite m_sprite;
	sf::Color m_blackPieceColor;
	sf::Color m_redPieceColor;
	sf::RectangleShape m_rect;
	std::vector<CheckerPiece> m_pieces;
	Checkerboard m_board;
	//std::vector<CheckerSquare> m_squares;
	//std::vector<CheckerSquare> m_capturedRedSquares;
	//std::vector<CheckerSquare> m_capturedBlackSquares;

	int m_currentPlayer;
	std::array<Player*, 2> m_players;

	//CheckerSquare* m_pSelectedSquare;
	//std::vector<CheckerSquare*> m_validMovesFromSelectedSquare;
	//bool m_jumpIsPossible;
	//bool m_validMoveIsJump;
	//// bool m_jumpAgain;
	std::stack<Command*> m_commands;

	bool isGameOver(CheckerColor& outWinningColor);
	/*CheckerSquare* getClickedSquare();
	bool capturePieceFromSquare(CheckerSquare& square);
	CheckerSquare* findJumpedSquare(CheckerSquare& from, CheckerSquare& to);
	Command* performRandomMovement(bool& didJump);*/

	/*enum CheckerSelectionProgress
	{
		// Player's turn just started, no piece is selected yet
		kNoCheckerSelected,

		// Player started holding down the left mouse button while hovering over a piece
		kCheckerGrabbed,

		// Player stopped holding down the left mouse button while a piece was grabbed
		kCheckerReleased,

		// Player grabbed and released a piece already, but the move was not valid
		kCheckerSelected
	};
	CheckerSelectionProgress m_selectionProgress;
	int m_selectedPiece;*/
};