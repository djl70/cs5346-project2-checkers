#include "checkers_game_state.h"

#include "config.h"
#include "game_over_state.h"
#include "main_menu_state.h"

//#include "move_command.h"
#include "ai_player.h"
#include "human_player.h"

CheckersGameState::CheckersGameState(ResourceManager* pResources)
	: m_pResources{ pResources }
{

}

void CheckersGameState::enter()
{
	m_background.setTexture(*m_pResources->getTexture("background"));
	m_background.setScale({ config::kScaling, config::kScaling });

	m_moveSound.setBuffer(*m_pResources->getSound("sound_move"));
	m_jumpSound.setBuffer(*m_pResources->getSound("sound_jump"));

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

	m_players[0] = new HumanPlayer{ kBlack };
	m_players[0]->setResources(m_pResources);
	m_players[0]->setBoard(&m_board);
	m_players[1] = new AIPlayer{ kRed };
	m_players[1]->setResources(m_pResources);
	m_players[1]->setBoard(&m_board);

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
				// Undo past 2 moves and have player take turn again (temporary fix for P1 vs COM)
				if (!m_commands.empty())
				{
					Command* pCommand = m_commands.top();
					m_commands.pop();
					pCommand->undo();
					delete pCommand;

					pCommand = m_commands.top();
					m_commands.pop();
					pCommand->undo();
					delete pCommand;

					m_players.at(m_currentPlayer)->takeTurn();
				}
			}
			break;
		}

		m_players.at(m_currentPlayer)->event(event);
	}

	FullMoveCommand* pCommand = m_players.at(m_currentPlayer)->update();
	if (pCommand)
	{
		if (m_currentPlayer == 1)
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
		m_moveSound.play();
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
	m_pResources->getWindow()->clear(sf::Color::White);

	m_pResources->getWindow()->draw(m_background);

	// Draw game board
	for (auto& square : m_board.board)
	{
		/*if (&square == m_pSelectedSquare) { continue; }
		bool drawLater = false;
		for (CheckerSquare* valid : m_validMovesFromSelectedSquare)
		{
			if (&square == valid)
			{
				drawLater = true;
				break;
			}
		}
		if (!drawLater)
		{
			square.render(m_resources.getWindow(), false);
		}*/
		square.render(m_pResources->getWindow());
	}

	// Draw selected squares later to draw them on top
	/*if (m_pSelectedSquare)
	{
		for (CheckerSquare* valid : m_validMovesFromSelectedSquare)
		{
			valid->render(m_resources.getWindow(), true);
		}
		m_pSelectedSquare->render(m_resources.getWindow(), true);
	}*/

	for (auto& square : m_board.capturedRedSquares)
	{
		square.render(m_pResources->getWindow());
	}

	for (auto& square : m_board.capturedBlackSquares)
	{
		square.render(m_pResources->getWindow());
	}

	// Draw pieces
	//for (const auto& piece : m_pieces)
	//{
	//	piece.render(resources::pWindow);
	//}

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

	delete m_players[0];
	delete m_players[1];

	while (m_moveSound.getStatus() == sf::Sound::Status::Playing)
	{

	}
	m_moveSound.resetBuffer();

	while (m_jumpSound.getStatus() == sf::Sound::Status::Playing)
	{

	}
	m_jumpSound.resetBuffer();
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

/*CheckerSquare* CheckersGameState::getClickedSquare()
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
		return &m_board.board[clickedSquare];
	}

	return nullptr;
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

CheckerSquare* CheckersGameState::findJumpedSquare(CheckerSquare& from, CheckerSquare& to)
{
	// Check north-west
	CheckerSquare* neighbor = to.getNeighbors().pNeighborNorthWest;
	if (neighbor && neighbor->getNeighbors().pNeighborNorthWest == &from)
	{
		return neighbor;
	}

	// Check north-east
	neighbor = to.getNeighbors().pNeighborNorthEast;
	if (neighbor && neighbor->getNeighbors().pNeighborNorthEast == &from)
	{
		return neighbor;
	}

	// Check south-west
	neighbor = to.getNeighbors().pNeighborSouthWest;
	if (neighbor && neighbor->getNeighbors().pNeighborSouthWest == &from)
	{
		return neighbor;
	}

	// Check south-east
	neighbor = to.getNeighbors().pNeighborSouthEast;
	if (neighbor && neighbor->getNeighbors().pNeighborSouthEast == &from)
	{
		return neighbor;
	}

	// Something went wrong at this point
	throw ("Error: Could not identify jumped square");

	sf::Vector2i toCoord = to.getPositionOnBoard();
	sf::Vector2i fromCoord = from.getPositionOnBoard();
	sf::Vector2i diff = toCoord - fromCoord;
	sf::Vector2i half = diff / 2;
	sf::Vector2i jumpedCoord = fromCoord + half;
	int index = jumpedCoord.y * 8 + jumpedCoord.x;
	return &m_board.board.at(index);
}

Command* CheckersGameState::performRandomMovement(bool& didJump)
{
	// First, check if a jump is globally possible
	std::vector<JumpInfo> jumps = findAllValidJumps(m_board, kRed);
	if (!jumps.empty())
	{
		didJump = true;
		return new JumpCommand(m_board, jumps[0]);
	}

	// No jump is possible, so pick a random movement
	std::vector<MoveInfo> moves = findAllValidMoves(m_board, kRed);
	didJump = false;
	return new MoveCommand(m_board, moves[0]);
}*/