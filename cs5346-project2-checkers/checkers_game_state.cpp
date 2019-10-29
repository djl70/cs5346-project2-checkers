#include "checkers_game_state.h"

#include "config.h"
#include "game_over_state.h"
#include "main_menu_state.h"

#include "move_command.h"

CheckersGameState::CheckersGameState(ResourceManager& resources)
	: m_resources{ resources }
{

}

void CheckersGameState::enter()
{
	m_background.setTexture(m_resources.getTexture("background"));
	m_background.setScale({ config::kScaling, config::kScaling });

	m_moveSound.setBuffer(m_resources.getSound("sound_move"));
	m_jumpSound.setBuffer(m_resources.getSound("sound_jump"));

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
					CheckerPiece piece(kRed, m_resources.getTexture("red_man"), m_resources.getTexture("red_king"));
					piece.setPosition(position);
					m_pieces.push_back(piece);
				}
				else if (r > 4)
				{
					CheckerPiece piece(kBlack, m_resources.getTexture("black_man"), m_resources.getTexture("black_king"));
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
			sf::Vector2f position{ config::boardTopLeft.x + config::kSquareWidth * c, config::boardTopLeft.y + config::kSquareWidth * r };
			if ((r + c) % 2 == 1)
			{
				CheckerSquare square(kBlackSquare);
				square.setPosition(position);
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
				CheckerSquare square(kRedSquare);
				square.setPosition(position);
				m_board.board.push_back(square);
			}
		}
	}

	// Set neighboring squares for black squares
	for (int r = 0; r < 8; ++r)
	{
		for (int c = 0; c < 8; ++c)
		{
			if ((r + c) % 2 == 1)
			{
				CheckerSquare& square = m_board.board[r * 8 + c];

				NeighboringSquares neighbors;
				if (r > 0 && c > 0)
				{
					neighbors.pNeighborNorthWest = &m_board.board[(r - 1) * 8 + (c - 1)];
				}
				if (r > 0 && c < 7)
				{
					neighbors.pNeighborNorthEast = &m_board.board[(r - 1) * 8 + (c + 1)];
				}
				if (r < 7 && c > 0)
				{
					neighbors.pNeighborSouthWest = &m_board.board[(r + 1) * 8 + (c - 1)];
				}
				if (r < 7 && c < 7)
				{
					neighbors.pNeighborSouthEast = &m_board.board[(r + 1) * 8 + (c + 1)];
				}

				square.setNeighbors(neighbors);
			}
		}
	}

	// Create squares for the captured areas
	for (int r = 0; r < 4; ++r)
	{
		for (int c = 0; c < 3; ++c)
		{
			sf::Vector2f redPosition{ config::capturedRedTopLeft.x + config::kSquareWidth * c, config::capturedRedTopLeft.y + config::kSquareWidth * r };
			CheckerSquare redSquare(kCapturedRed);
			redSquare.setPosition(redPosition);
			redSquare.setPromotionColor(kRed);
			m_board.capturedRedSquares.push_back(redSquare);

			sf::Vector2f blackPosition{ config::capturedBlackTopLeft.x + config::kSquareWidth * c, config::capturedBlackTopLeft.y + config::kSquareWidth * r };
			CheckerSquare blackSquare(kCapturedBlack);
			blackSquare.setPosition(blackPosition);
			blackSquare.setPromotionColor(kBlack);
			m_board.capturedBlackSquares.push_back(blackSquare);
		}
	}

	m_selectionProgress = kNoCheckerSelected;
	m_selectedPiece = -1;
}

BaseState* CheckersGameState::event()
{
	sf::Event event;
	while (m_resources.getWindow()->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_resources.getWindow()->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape)
			{
				return new MainMenuState(m_resources);
			}
			break;
		case sf::Event::MouseButtonPressed:
			// For testing purposes, capture all 5 pieces around the left mouse click
			if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				int clickedSquare = -1;
				for (int i = 0; i < m_board.board.size(); ++i)
				{
					if (m_board.board[i].contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_resources.getWindow()))))
					{
						clickedSquare = i;
						break;
					}
				}
				if (clickedSquare >= 0)
				{
					capturePieceFromSquare(m_board.board[clickedSquare]);
					NeighboringSquares neighbors = m_board.board[clickedSquare].getNeighbors();
					if (neighbors.pNeighborNorthEast)
					{
						capturePieceFromSquare(*neighbors.pNeighborNorthEast);
					}
					if (neighbors.pNeighborNorthWest)
					{
						capturePieceFromSquare(*neighbors.pNeighborNorthWest);
					}
					if (neighbors.pNeighborSouthEast)
					{
						capturePieceFromSquare(*neighbors.pNeighborSouthEast);
					}
					if (neighbors.pNeighborSouthWest)
					{
						capturePieceFromSquare(*neighbors.pNeighborSouthWest);
					}
				}
			}
			// For testing purposes, move the piece south-west for red and north-east for black on right mouse click
			else if (event.mouseButton.button == sf::Mouse::Button::Right)
			{
				int clickedSquare = -1;
				for (int i = 0; i < m_board.board.size(); ++i)
				{
					if (m_board.board[i].contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_resources.getWindow()))))
					{
						clickedSquare = i;
						break;
					}
				}
				if (clickedSquare >= 0)
				{
					CheckerPiece* piece = m_board.board[clickedSquare].getPiece();
					if (piece)
					{
						if (piece->getColor() == kRed)
						{
							// Move south-west if empty
							CheckerSquare& from = m_board.board[clickedSquare];
							CheckerSquare* to = from.getNeighbors().pNeighborSouthWest;
							if (to && !to->getPiece())
							{
								Command* pCommand = new MoveCommand(m_board, { from, *to });
								pCommand->execute();
								m_commands.push(pCommand);
							}
						}
						else if (piece->getColor() == kBlack)
						{
							// Move north-east
							CheckerSquare& from = m_board.board[clickedSquare];
							CheckerSquare* to = from.getNeighbors().pNeighborNorthEast;
							if (to && !to->getPiece())
							{
								Command* pCommand = new MoveCommand(m_board, { from, *to });
								pCommand->execute();
								m_commands.push(pCommand);
							}
						}
					}
				}
			}
			// Undo the previous command when middle mouse button pressed
			else if (event.mouseButton.button == sf::Mouse::Button::Middle)
			{
				if (!m_commands.empty())
				{
					Command* pCommand = m_commands.top();
					m_commands.pop();
					pCommand->undo();
					delete pCommand;
				}
			}
			break;
		}
	}

	CheckerColor winningColor;
	if (isGameOver(winningColor))
	{
		return new GameOverState(m_resources, winningColor);
	}

	return nullptr;
}

void CheckersGameState::render()
{
	m_resources.getWindow()->clear(sf::Color::White);

	m_resources.getWindow()->draw(m_background);

	// Draw game board
	for (const auto& square : m_board.board)
	{
		square.render(m_resources.getWindow());
	}

	for (const auto& square : m_board.capturedRedSquares)
	{
		square.render(m_resources.getWindow());
	}

	for (const auto& square : m_board.capturedBlackSquares)
	{
		square.render(m_resources.getWindow());
	}

	// Draw pieces
	//for (const auto& piece : m_pieces)
	//{
	//	piece.render(resources::pWindow);
	//}

	m_resources.getWindow()->display();
}

void CheckersGameState::exit()
{
	while (m_moveSound.getStatus() == sf::Sound::Status::Playing)
	{

	}
	m_moveSound.resetBuffer();

	while (m_jumpSound.getStatus() == sf::Sound::Status::Playing)
	{

	}
	m_jumpSound.resetBuffer();
}

bool CheckersGameState::isGameOver(CheckerColor& outWinningColor) const
{
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
}

bool CheckersGameState::capturePieceFromSquare(CheckerSquare& square)
{
	CheckerPiece* piece = square.getPiece();

	if (piece)
	{
		switch (piece->getColor())
		{
		case kBlack:
			for (int i = 0; i < m_board.capturedBlackSquares.size(); ++i)
			{
				if (m_board.capturedBlackSquares[i].isEmpty())
				{
					m_moveSound.play();
					m_board.capturedBlackSquares[i].setPiece(piece);
					break;
				}
			}
			break;
		case kRed:
			for (int i = 0; i < m_board.capturedRedSquares.size(); ++i)
			{
				if (m_board.capturedRedSquares[i].isEmpty())
				{
					m_jumpSound.play();
					m_board.capturedRedSquares[i].setPiece(piece);
					break;
				}
			}
			break;
		}
		square.setPiece(nullptr);
		return true;
	}

	return false;
}