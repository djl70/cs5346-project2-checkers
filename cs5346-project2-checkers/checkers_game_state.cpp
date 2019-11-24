#include "checkers_game_state.h"

#include "config.h"
#include "game_over_state.h"
#include "main_menu_state.h"

#include "ai_player.h"
#include "human_player.h"

#include <iostream>

CheckersGameState::CheckersGameState(ResourceManager* pResources, Player* firstPlayer, Player* secondPlayer)
	: m_pResources{ pResources }
	, m_players{ firstPlayer, secondPlayer }
	, m_turnIndicatorSquares{ CheckerSquare{ kCapturedBlack }, CheckerSquare{ kCapturedBlack } }
{
	m_turnIndicators.at(0).setColor(firstPlayer->getColor());
	m_turnIndicators.at(0).setTextures(m_pResources->getTexture(firstPlayer->getColor() == kBlack ? "black_man" : "red_man"), m_pResources->getTexture(firstPlayer->getColor() == kBlack ? "black_king" : "red_king"));
	
	m_turnIndicators.at(1).setColor(secondPlayer->getColor());
	m_turnIndicators.at(1).setTextures(m_pResources->getTexture(secondPlayer->getColor() == kBlack ? "black_man" : "red_man"), m_pResources->getTexture(secondPlayer->getColor() == kBlack ? "black_king" : "red_king"));
	
	for (int i = 0; i < m_players.size(); ++i)
	{
		CheckerColor color = m_turnIndicators.at(i).getColor();
		const sf::Vector2f& point = color == kBlack ? config::blackTurnIndicatorTopLeft : config::redTurnIndicatorTopLeft;
		m_turnIndicators.at(i).setPosition(point);
		m_turnIndicatorSquares.at(i).setPositionOnBoard({ 0, 0 }, point);
	}
}

CheckersGameState::~CheckersGameState()
{
	while (!m_commands.empty())
	{
		delete m_commands.top();
		m_commands.pop();
	}

	for (auto& player : m_players)
	{
		delete player;
		player = nullptr;
	}
}

void CheckersGameState::enter()
{
	m_background.setTexture(*m_pResources->getTexture("background"));
	m_background.setScale({ config::kScaling, config::kScaling });

	// Create pieces
	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			if ((r + c) % 2 == 1)
			{
				if (r < 3)
				{
					CheckerPiece piece;
					piece.setColor(kRed);
					piece.setTextures(m_pResources->getTexture("red_man"), m_pResources->getTexture("red_king"));
					m_board.pieces.push_back(piece);
				}
				else if (r > 4)
				{
					CheckerPiece piece;
					piece.setColor(kBlack);
					piece.setTextures(m_pResources->getTexture("black_man"), m_pResources->getTexture("black_king"));
					m_board.pieces.push_back(piece);
				}
			}
		}
	}

	// Create squares
	int p = 0;
	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			if ((r + c) % 2 == 1)
			{
				CheckerSquare square{ kBlackSquare };
				square.setPositionOnBoard({ c, r }, config::boardTopLeft);
				m_board.board.push_back(square);
				if (r < 3)
				{
					if (r == 0)
					{
						m_board.board.back().setPromotionColor(kBlack);
					}
					checkerboard::placePieceAt(m_board, p++, m_board.board.size() - 1);
				}
				else if (r > 4)
				{
					if (r == 7)
					{
						m_board.board.back().setPromotionColor(kRed);
					}
					checkerboard::placePieceAt(m_board, p++, m_board.board.size() - 1);
				}
			}
			else
			{
				CheckerSquare square{ kRedSquare };
				square.setPositionOnBoard({ c, r }, config::boardTopLeft);
				m_board.board.push_back(square);
			}
		}
	}

	// Create squares for the captured areas
	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 3; ++c)
		{
			CheckerSquare redSquare{ kCapturedRed };
			redSquare.setPositionOnBoard({ c, r }, config::capturedRedTopLeft);
			m_board.capturedRedSquares.push_back(redSquare);

			CheckerSquare blackSquare{ kCapturedBlack };
			blackSquare.setPositionOnBoard({ c, r }, config::capturedBlackTopLeft);
			m_board.capturedBlackSquares.push_back(blackSquare);
		}
	}

	for (auto& player : m_players)
	{
		player->setResources(m_pResources);
		player->setBoard(&m_board);
	}

	m_players[m_board.currentPlayer]->startTurn();
}

BaseState* CheckersGameState::event()
{
	sf::Event event;
	while (m_pResources->getWindow()->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_pResources->getWindow()->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape)
			{
				return new MainMenuState{ m_pResources };
			}
			break;
		case sf::Event::MouseButtonPressed:
			// Undo the previous command when right mouse button pressed
			if (event.mouseButton.button == sf::Mouse::Button::Right)
			{
				if (!m_commands.empty())
				{
					// If both players are AIs, don't undo anything
					// If both players are humans, undo only one move
					// If one player is human and the other is an AI, undo two moves
					int numBots = 0;
					for (const auto& player : m_players)
					{
						numBots += player->isBot();
					}

					if (numBots == 0)
					{
						// Undo the last move
						Command* pCommand = m_commands.top();
						m_commands.pop();
						pCommand->undo();
						delete pCommand;

						// Switch players
						m_players.at(m_board.currentPlayer)->startTurn();
					}
					else if (numBots == 1 && !m_players.at(m_board.currentPlayer)->isBot())
					{
						// Undo the last 2 moves, but only if it's the human player's turn
						Command* pCommand = m_commands.top();
						m_commands.pop();
						pCommand->undo();
						delete pCommand;

						pCommand = m_commands.top();
						m_commands.pop();
						pCommand->undo();
						delete pCommand;

						// Re-take the current player's turn
						m_players.at(m_board.currentPlayer)->startTurn();
					}
				}
			}
			break;
		}

		m_players.at(m_board.currentPlayer)->event(event);
	}

	FullMoveCommand* pCommand = m_players.at(m_board.currentPlayer)->update();
	if (pCommand)
	{
		pCommand->execute();
		m_commands.push(pCommand);

		if (pCommand->didPromote())
		{
			m_pResources->playSound("sound_jump");
		}
		else
		{
			m_pResources->playSound("sound_move");
		}

		GameOverCondition gameOverCondition;
		if (checkerboard::isGameOver(m_board, gameOverCondition))
		{
			std::cout << "\nGame over after " << m_board.turnNumber << " moves\n";
			switch (gameOverCondition)
			{
			case kBlackCannotMove:
			case kBlackHasNoPiecesLeft:
				std::cout << "P2 won\n";
				break;
			case kRedCannotMove:
			case kRedHasNoPiecesLeft:
				std::cout << "P1 won\n";
				break;
			case kBoardStateRepetitionLimitReached:
			case kTurnLimitReached:
				std::cout << "Draw\n";
				break;
			}
			return new GameOverState{ m_pResources, gameOverCondition };
		}

		m_players.at(m_board.currentPlayer)->startTurn();
	}

	return nullptr;
}

void CheckersGameState::render()
{
	m_pResources->getWindow()->clear(sf::Color::White);

	m_pResources->getWindow()->draw(m_background);

	// Draw turn indicators
	for (int i = 0; i < m_players.size(); ++i)
	{
		m_turnIndicatorSquares.at(i).render(m_pResources->getWindow());
		if (m_board.currentPlayer == i)
		{
			m_turnIndicators.at(i).promote();
			m_turnIndicatorSquares.at(i).renderHighlight(m_pResources->getWindow());
		}
		else
		{
			m_turnIndicators.at(i).demote();
		}
		m_turnIndicators.at(i).render(m_pResources->getWindow());
	}

	// Give the player the responsibility of drawing the game board
	m_players.at(m_board.currentPlayer)->render(m_pResources->getWindow());

	m_pResources->getWindow()->display();
}