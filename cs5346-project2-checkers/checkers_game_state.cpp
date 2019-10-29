#include "checkers_game_state.h"

#include "config.h"
#include "game_over_state.h"
#include "main_menu_state.h"
#include "resources.h"

void CheckersGameState::enter()
{
	m_background.setTexture(resources::textures["background"]);
	m_background.setScale({ config::kScaling, config::kScaling });

	m_moveSound.setBuffer(resources::sounds["sound_move"]);
	m_jumpSound.setBuffer(resources::sounds["sound_jump"]);

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
					CheckerPiece piece(kRed);
					piece.setPosition(position);
					m_pieces.push_back(piece);
				}
				else if (r > 4)
				{
					CheckerPiece piece(kBlack);
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
				m_squares.push_back(square);
				if (r < 3)
				{
					if (r == 0)
					{
						m_squares.back().setPromotionColor(kBlack);
					}
					m_squares.back().setPiece(&m_pieces[p++]);
				}
				else if (r > 4)
				{
					if (r == 7)
					{
						m_squares.back().setPromotionColor(kRed);
					}
					m_squares.back().setPiece(&m_pieces[p++]);
				}
			}
			else
			{
				CheckerSquare square(kRedSquare);
				square.setPosition(position);
				m_squares.push_back(square);
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
			m_capturedRedSquares.push_back(redSquare);

			sf::Vector2f blackPosition{ config::capturedBlackTopLeft.x + config::kSquareWidth * c, config::capturedBlackTopLeft.y + config::kSquareWidth * r };
			CheckerSquare blackSquare(kCapturedBlack);
			blackSquare.setPosition(blackPosition);
			blackSquare.setPromotionColor(kBlack);
			m_capturedBlackSquares.push_back(blackSquare);
		}
	}

	m_selectionProgress = kNoCheckerSelected;
	m_selectedPiece = -1;
}

BaseState* CheckersGameState::event()
{
	sf::Event event;
	while (resources::pWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			resources::pWindow->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape)
			{
				return new MainMenuState;
			}
			break;
		case sf::Event::MouseButtonPressed:
			// Select a piece or move an already selected piece

			// First, see which piece was clicked, if any
			int clickedSquare = -1;
			for (int i = 0; i < m_squares.size(); ++i)
			{
				if (m_squares[i].contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*resources::pWindow))))
				{
					clickedSquare = i;
					break;
				}
			}
			if (clickedSquare >= 0)
			{
				CheckerPiece* piece = m_squares[clickedSquare].getPiece();
				if (piece)
				{
					switch (piece->getColor())
					{
					case kBlack:
						for (int i = 0; i < m_capturedBlackSquares.size(); ++i)
						{
							if (m_capturedBlackSquares[i].isEmpty())
							{
								m_moveSound.play();
								m_capturedBlackSquares[i].setPiece(piece);
								break;
							}
						}
						break;
					case kRed:
						for (int i = 0; i < m_capturedRedSquares.size(); ++i)
						{
							if (m_capturedRedSquares[i].isEmpty())
							{
								m_jumpSound.play();
								m_capturedRedSquares[i].setPiece(piece);
								break;
							}
						}
						break;
					}
					m_squares[clickedSquare].setPiece(nullptr);
				}
			}
			break;
		}
	}

	CheckerColor winningColor;
	if (isGameOver(winningColor))
	{
		return new GameOverState(winningColor);
	}

	return nullptr;
}

void CheckersGameState::render()
{
	resources::pWindow->clear(sf::Color::White);

	resources::pWindow->draw(m_background);

	// Draw game board
	for (const auto& square : m_squares)
	{
		square.render(resources::pWindow);
	}

	for (const auto& square : m_capturedRedSquares)
	{
		square.render(resources::pWindow);
	}

	for (const auto& square : m_capturedBlackSquares)
	{
		square.render(resources::pWindow);
	}

	// Draw pieces
	//for (const auto& piece : m_pieces)
	//{
	//	piece.render(resources::pWindow);
	//}

	resources::pWindow->display();
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
	for (const auto& square : m_capturedRedSquares)
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
	for (const auto& square : m_capturedBlackSquares)
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