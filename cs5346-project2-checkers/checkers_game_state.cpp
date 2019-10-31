#include "checkers_game_state.h"

#include "config.h"
#include "game_over_state.h"
#include "main_menu_state.h"

//#include "move_command.h"
#include "ai_player.h"
#include "human_player.h"

CheckersGameState::CheckersGameState(ResourceManager* pResources, Player* firstPlayer, Player* secondPlayer)
	: m_pResources{ pResources }
	, m_players{ firstPlayer, secondPlayer }
{

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
			sf::Vector2f position{ config::boardTopLeft.x + config::kSquareWidth * c, config::boardTopLeft.y + config::kSquareWidth * r };
			if ((r + c) % 2 == 1)
			{
				if (r < 3)
				{
					CheckerPiece piece{ kRed, m_pResources->getTexture("red_man"), m_pResources->getTexture("red_king") };
					piece.setPosition(position);
					m_pieces.push_back(piece);
				}
				else if (r > 4)
				{
					CheckerPiece piece{ kBlack, m_pResources->getTexture("black_man"), m_pResources->getTexture("black_king") };
					piece.setPosition(position);
					m_pieces.push_back(piece);
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
			//sf::Vector2f position{ config::boardTopLeft.x + config::kSquareWidth * c, config::boardTopLeft.y + config::kSquareWidth * r };
			if ((r + c) % 2 == 1)
			{
				CheckerSquare square{ kBlackSquare };
				//square.setPosition(position);
				square.setPositionOnBoard({ c, r }, config::boardTopLeft);
				m_board.board.push_back(square);
				if (r < 3)
				{
					if (r == 0)
					{
						m_board.board.back().setPromotionColor(kBlack);
					}
					m_board.board.back().setPiece(&m_pieces[p++]);
				}
				else if (r > 4)
				{
					if (r == 7)
					{
						m_board.board.back().setPromotionColor(kRed);
					}
					m_board.board.back().setPiece(&m_pieces[p++]);
				}
			}
			else
			{
				CheckerSquare square{ kRedSquare };
				//square.setPosition(position);
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
			//sf::Vector2f redPosition{ config::capturedRedTopLeft.x + config::kSquareWidth * c, config::capturedRedTopLeft.y + config::kSquareWidth * r };
			CheckerSquare redSquare{ kCapturedRed };
			redSquare.setPositionOnBoard({ c, r }, config::capturedRedTopLeft);
			m_board.capturedRedSquares.push_back(redSquare);

			//sf::Vector2f blackPosition{ config::capturedBlackTopLeft.x + config::kSquareWidth * c, config::capturedBlackTopLeft.y + config::kSquareWidth * r };
			CheckerSquare blackSquare{ kCapturedBlack };
			blackSquare.setPositionOnBoard({ c, r }, config::capturedBlackTopLeft);
			m_board.capturedBlackSquares.push_back(blackSquare);
		}
	}

	//m_players[0] = new HumanPlayer{ kBlack, false };
	//m_players[1] = new AIPlayer{ kRed, true };
	for (auto& player : m_players)
	{
		player->setResources(m_pResources);
		player->setBoard(&m_board);
	}

	m_currentPlayer = 0;
	m_players[m_currentPlayer]->takeTurn();
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
						m_currentPlayer = (m_currentPlayer + 1) % 2;
						m_players.at(m_currentPlayer)->takeTurn();
					}
					else if (numBots == 1)
					{
						// Undo the last 2 moves
						Command* pCommand = m_commands.top();
						m_commands.pop();
						pCommand->undo();
						delete pCommand;

						pCommand = m_commands.top();
						m_commands.pop();
						pCommand->undo();
						delete pCommand;

						// Re-take the current player's turn
						m_players.at(m_currentPlayer)->takeTurn();
					}
				}
			}
			break;
		}

		m_players.at(m_currentPlayer)->event(event);
	}

	FullMoveCommand* pCommand = m_players.at(m_currentPlayer)->update();
	if (pCommand)
	{
		if (m_players.at(m_currentPlayer)->isBot())
		{
			// TODO: Fix this so that it pauses between steps
			do
			{
				sf::sleep(sf::milliseconds(250));
			} while (!pCommand->executeStep());
		}
		else
		{
			pCommand->execute();
		}
		m_commands.push(pCommand);

		if (pCommand->didPromote())
		{
			m_pResources->playSound("sound_jump");
		}
		else
		{
			m_pResources->playSound("sound_move");
		}

		// TODO: End the player's turn here, because we're only dealing with full moves now. Don't let the player decide when their turn is over.
	}

	if (!m_players.at(m_currentPlayer)->isTurn())
	{
		m_currentPlayer = (m_currentPlayer + 1) % 2;
		m_players.at(m_currentPlayer)->takeTurn();
	}

	// TODO: Only check if no moves are available for the current player (since we just switched players by this point)
	// Also, check if either captured area is full of pieces
	CheckerColor winningColor;
	if (isGameOver(winningColor))
	{
		return new GameOverState{ m_pResources, winningColor };
	}

	return nullptr;
}

void CheckersGameState::render()
{
	// TODO: Above the captured areas, draw the respective piece (kinged) and highlight its square when it's that player's turn.

	m_pResources->getWindow()->clear(sf::Color::White);

	m_pResources->getWindow()->draw(m_background);

	// Draw game board
	for (auto& square : m_board.board)
	{
		square.render(m_pResources->getWindow());
	}

	for (auto& square : m_board.capturedRedSquares)
	{
		square.render(m_pResources->getWindow());
	}

	for (auto& square : m_board.capturedBlackSquares)
	{
		square.render(m_pResources->getWindow());
	}

	m_players[m_currentPlayer]->render(m_pResources->getWindow());

	m_pResources->getWindow()->display();
}

void CheckersGameState::exit()
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

bool CheckersGameState::isGameOver(CheckerColor& outWinningColor)
{
	/*
	// Check for black win
	bool isBlackWin = true;
	for (const auto& square : m_board.capturedRedSquares)
	{
		if (square.isEmpty())
		{
			isBlackWin = false;
			break;
		}
	}
	if (isBlackWin)
	{
		outWinningColor = kBlack;
		return true;
	}

	// Check for red win
	bool isRedWin = true;
	for (const auto& square : m_board.capturedBlackSquares)
	{
		if (square.isEmpty())
		{
			isRedWin = false;
			break;
		}
	}
	if (isRedWin)
	{
		outWinningColor = kRed;
		return true;
	}

	return false;
	*/

	if (findAllValidMoves(m_board, kRed).empty())
	{
		outWinningColor = kBlack;
		return true;
	}
	else if (findAllValidMoves(m_board, kBlack).empty())
	{
		outWinningColor = kRed;
		return true;
	}

	return false;
}