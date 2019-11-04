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
	//, m_turnIndicators{
	//	CheckerPiece{ firstPlayer->getColor(), pResources->getTexture(firstPlayer->getColor() == kBlack ? "black_man" : "red_man"), pResources->getTexture(firstPlayer->getColor() == kBlack ? "black_king" : "red_king") },
	//	CheckerPiece{ secondPlayer->getColor(), pResources->getTexture(secondPlayer->getColor() == kBlack ? "black_man" : "red_man"), pResources->getTexture(secondPlayer->getColor() == kBlack ? "black_king" : "red_king") }
	//}
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

void CheckersGameState::enter()
{
	m_background.setTexture(*m_pResources->getTexture("background"));
	m_background.setScale({ config::kScaling, config::kScaling });

	// Create pieces
	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			//sf::Vector2f position{ config::boardTopLeft.x + config::kSquareWidth * c, config::boardTopLeft.y + config::kSquareWidth * r };
			if ((r + c) % 2 == 1)
			{
				if (r < 3)
				{
					//CheckerPiece piece{ kRed, m_pResources->getTexture("red_man"), m_pResources->getTexture("red_king") };
					CheckerPiece piece;
					piece.setColor(kRed);
					piece.setTextures(m_pResources->getTexture("red_man"), m_pResources->getTexture("red_king"));
					//piece.setPosition(position);
					m_board.pieces.push_back(piece);
				}
				else if (r > 4)
				{
					//CheckerPiece piece{ kBlack, m_pResources->getTexture("black_man"), m_pResources->getTexture("black_king") };
					CheckerPiece piece;
					piece.setColor(kBlack);
					piece.setTextures(m_pResources->getTexture("black_man"), m_pResources->getTexture("black_king"));
					//piece.setPosition(position);
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
					checkerboard::placePieceAt(m_board, p++, m_board.board.size() - 1);
					//m_board.board.back().setPieceIndex(p++);
				}
				else if (r > 4)
				{
					if (r == 7)
					{
						m_board.board.back().setPromotionColor(kRed);
					}
					checkerboard::placePieceAt(m_board, p++, m_board.board.size() - 1);
					//m_board.board.back().setPieceIndex(p++);
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

	//m_currentPlayer = 0;
	m_players[m_board.currentPlayer]->startTurn();
	//m_numMovesSinceCaptureOrKinging = 0;
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
						//// Since we are undoing, we want to decrement the number of times we have encountered the current board state
						//--m_boardStateFrequency.at(checkerboard::encode(m_board, m_currentPlayer));
						
						// Undo the last move
						Command* pCommand = m_commands.top();
						m_commands.pop();
						pCommand->undo();
						delete pCommand;

						// Switch players
						//m_currentPlayer = (m_currentPlayer + 1) % 2;
						m_players.at(m_board.currentPlayer)->startTurn();
					}
					else if (numBots == 1 && !m_players.at(m_board.currentPlayer)->isBot())
					{
						//// Since we are undoing twice, we want to decrement the number of times we have encountered both the current and previous board states
						//--m_boardStateFrequency.at(checkerboard::encode(m_board, m_currentPlayer));

						// Undo the last 2 moves, but only if it's the human player's turn
						Command* pCommand = m_commands.top();
						m_commands.pop();
						pCommand->undo();
						delete pCommand;

						//--m_boardStateFrequency.at(checkerboard::encode(m_board, (m_currentPlayer + 1) % 2));

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

		//// Reset the moves counter to 0 if we jumped or kinged a piece
		//if (pCommand->isJump() || pCommand->didPromote())
		//{
		//	m_numMovesSinceCaptureOrKinging = 0;
		//}
		//else
		//{
		//	++m_numMovesSinceCaptureOrKinging;
		//}

		//// End the player's turn here, because we're only dealing with full moves now. Don't let the player decide when their turn is over.
		//m_currentPlayer = (m_currentPlayer + 1) % 2;

		//// Increment the number of times we've encountered the current board state
		//++m_boardStateFrequency[checkerboard::encode(m_board, m_currentPlayer)];

		GameOverCondition gameOverCondition;
		if (isGameOver(gameOverCondition))
		{
			return new GameOverState{ m_pResources, gameOverCondition };
		}

		m_players.at(m_board.currentPlayer)->startTurn();
	}

	//if (!m_players.at(m_currentPlayer)->isTurn())
	//{
	//	m_currentPlayer = (m_currentPlayer + 1) % 2;
	//	m_players.at(m_currentPlayer)->takeTurn();
	//}

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

	//// Draw game board
	//for (auto& square : m_board.board)
	//{
	//	square.render(m_pResources->getWindow());
	//}

	//for (auto& square : m_board.capturedRedSquares)
	//{
	//	square.render(m_pResources->getWindow());
	//}

	//for (auto& square : m_board.capturedBlackSquares)
	//{
	//	square.render(m_pResources->getWindow());
	//}

	//for (auto& piece : m_board.pieces)
	//{
	//	piece.render(m_pResources->getWindow());
	//}

	// Give the player the the responsibility of drawing the game board
	m_players.at(m_board.currentPlayer)->render(m_pResources->getWindow());

	m_pResources->getWindow()->display();
}

void CheckersGameState::exit()
{
	// We need to call stop here for the AI so that we can join its move selection thread.
	// Without doing so, the program will throw an exception if we exit this state while the AI is selecting a move.
	m_players.at(m_board.currentPlayer)->stop();

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

bool CheckersGameState::isGameOver(GameOverCondition& outGameOverCondition)
{
	// Check if black has captured all of red's pieces
	bool isRedOutOfPieces = true;
	for (const auto& square : m_board.capturedRedSquares)
	{
		if (square.isEmpty())
		{
			isRedOutOfPieces = false;
			break;
		}
	}
	if (isRedOutOfPieces)
	{
		outGameOverCondition = kRedHasNoPiecesLeft;
		return true;
	}

	// Check if red has captured all of black's pieces
	bool isBlackOutOfPieces = true;
	for (const auto& square : m_board.capturedBlackSquares)
	{
		if (square.isEmpty())
		{
			isBlackOutOfPieces = false;
			break;
		}
	}
	if (isBlackOutOfPieces)
	{
		outGameOverCondition = kBlackHasNoPiecesLeft;
		return true;
	}

	// Both players have at least one piece on the board, so check if the next player to move has any moves available
	CheckerColor playerColor = m_players.at(m_board.currentPlayer)->getColor();
	if (findAllValidFullMoves(m_board, playerColor).empty())
	{
		outGameOverCondition = playerColor == kBlack ? kBlackCannotMove : kRedCannotMove;
		return true;
	}

	// A move can be made, so check if the turn limit has been reached
	if (m_board.numTurnsSinceCaptureOrKinging >= config::kNumMovesWithoutCaptureOrKingingNeededToDraw)
	{
		outGameOverCondition = kTurnLimitReached;
		return true;
	}

	// The turn limit hasn't been reached, so check if we have encountered the current board state too many times
	for (const auto& pair : m_board.boardStateFrequency)
	{
		if (pair.second >= config::kBoardStateFrequencyNeededToDraw)
		{
			outGameOverCondition = kBoardStateRepetitionLimitReached;
			return true;
		}
	}

	// If we reach this point, the game is not over yet
	return false;
}