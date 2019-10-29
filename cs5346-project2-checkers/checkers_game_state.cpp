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

	//m_selectionProgress = kNoCheckerSelected;
	//m_selectedPiece = -1;
	m_pSelectedSquare = nullptr;
	m_validMoveIsJump = false;
	m_jumpIsPossible = false;
	// m_jumpAgain = false;
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
			// For testing purposes, capture all 5 pieces around the right mouse click
			if (event.mouseButton.button == sf::Mouse::Button::Right)
			{
				CheckerSquare* clickedSquare = getClickedSquare();
				if (clickedSquare)
				{
					capturePieceFromSquare(*clickedSquare);
					NeighboringSquares neighbors = clickedSquare->getNeighbors();
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
			// For testing purposes, perform automatic movements on left mouse click
			/*else if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				CheckerSquare* clickedSquare = getClickedSquare();
				if (clickedSquare)
				{
					CheckerPiece* piece = clickedSquare->getPiece();
					if (piece)
					{
						// Move to one of the neighbors, if possible
						std::vector<MoveInfo> moves = findValidMoves(*clickedSquare, piece->getColor());
						if (!moves.empty())
						{
							Command* pCommand = new MoveCommand(m_board, moves[0]);
							pCommand->execute();
							m_commands.push(pCommand);

							// Now, we'll have the other color move once
							moves = findAllValidMoves(m_board, piece->getColor() == kRed ? kBlack : kRed);
							if (!moves.empty())
							{
								pCommand = new MoveCommand(m_board, moves[0]);
								pCommand->execute();
								m_commands.push(pCommand);
							}
						}
					}
				}
			}*/
			// Player can only move black
			// If no square is selected, highlight the clicked square and the potential moves
			// If a square is selected, check if it would be a valid move from the selected square
			// TODO: If a jump is possible, force it. On a jump, remove the jumped piece. After a jump, check for other possible jumps and force them.
			else if (event.mouseButton.button == sf::Mouse::Button::Left)
			{
				bool clearSelectedSquare = true;
				bool moved = false;
				bool jumped = false;
				CheckerSquare* clickedSquare = getClickedSquare();
				if (clickedSquare)
				{
					// Determine if a jump is at all possible
					std::vector<JumpInfo> globalJumps = findAllValidJumps(m_board, kBlack);
					m_jumpIsPossible = !globalJumps.empty();

					CheckerPiece* piece = clickedSquare->getPiece();
					// New selection
					//if (!m_pSelectedSquare && piece && piece->getColor() == kBlack)
					if (piece && piece->getColor() == kBlack)
					{
						// First, check if a jump is possible
						std::vector<JumpInfo> jumps = findValidJumps(*clickedSquare, kBlack);
						if (jumps.empty() && !m_jumpIsPossible)
						{
							// No jump possible; check moves
							std::vector<MoveInfo> moves = findValidMoves(*clickedSquare, kBlack);
							// if (!moves.empty())
							// {
							m_validMovesFromSelectedSquare.clear();
							for (auto& info : moves)
							{
								m_validMovesFromSelectedSquare.push_back(&info.to);
							}
							m_pSelectedSquare = clickedSquare;
							m_validMoveIsJump = false;
							clearSelectedSquare = false;
							// }
						}
						else
						{
							// A jump is possible, so force it
							m_validMovesFromSelectedSquare.clear();
							for (auto& info : jumps)
							{
								m_validMovesFromSelectedSquare.push_back(&info.to);
							}
							m_pSelectedSquare = clickedSquare;
							m_validMoveIsJump = true;
							clearSelectedSquare = false;
						}
					}
					// Attempt to perform a move
					else if (m_pSelectedSquare && !piece)
					{
						for (CheckerSquare* square : m_validMovesFromSelectedSquare)
						{
							if (square == clickedSquare)
							{
								if (m_validMoveIsJump)
								{
									// Figure out which square is being jumped
									CheckerSquare* jumpedSquare = findJumpedSquare(*m_pSelectedSquare, *square);
									Command* pCommand = new JumpCommand(m_board, { *m_pSelectedSquare, *square, *jumpedSquare });
									pCommand->execute();
									m_commands.push(pCommand);
									m_pSelectedSquare = nullptr;
									m_validMovesFromSelectedSquare.clear();
									m_validMoveIsJump = false;
									jumped = true;
								}
								else
								{
									Command* pCommand = new MoveCommand(m_board, { *m_pSelectedSquare, *square });
									pCommand->execute();
									m_commands.push(pCommand);
									m_pSelectedSquare = nullptr;
									m_validMovesFromSelectedSquare.clear();
									m_validMoveIsJump = false;
									moved = true;
								}
								break;
							}
						}
					}
				}

				if (clearSelectedSquare)
				{
					m_validMovesFromSelectedSquare.clear();
					m_pSelectedSquare = nullptr;
				}

				if (jumped)
				{
					// Check if another jump is possible; if not, continue to the opponent's turn
					std::vector<JumpInfo> globalJumps = findAllValidJumps(m_board, kBlack);
					m_jumpIsPossible = !globalJumps.empty();
					m_validMoveIsJump = true;
					moved = !m_jumpIsPossible;

					//// First, check if a jump is possible
					//std::vector<JumpInfo> jumps = findValidJumps(*clickedSquare, kBlack);
					//if (!jumps.empty())
					//{
					//	// A jump is possible, so force it
					//	m_validMovesFromSelectedSquare.clear();
					//	for (auto& info : jumps)
					//	{
					//		m_validMovesFromSelectedSquare.push_back(&info.to);
					//	}
					//	m_pSelectedSquare = clickedSquare;
					//	m_validMoveIsJump = true;
					//	m_jumpAgain = true;
					//	moved = false; // prevent opponent from going next
					//	clearSelectedSquare = false;
					//}
				}

				// Opponent's turn
				if (moved)
				{
					std::vector<MoveInfo> moves = findAllValidMoves(m_board, kRed);
					if (!moves.empty())
					{
						Command* pCommand = new MoveCommand(m_board, moves[0]);
						pCommand->execute();
						m_commands.push(pCommand);
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
	for (auto& square : m_board.board)
	{
		if (&square == m_pSelectedSquare) { continue; }
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
		}
	}

	// Draw selected squares later to draw them on top
	if (m_pSelectedSquare)
	{
		for (CheckerSquare* valid : m_validMovesFromSelectedSquare)
		{
			valid->render(m_resources.getWindow(), true);
		}
		m_pSelectedSquare->render(m_resources.getWindow(), true);
	}

	for (auto& square : m_board.capturedRedSquares)
	{
		square.render(m_resources.getWindow(), false);
	}

	for (auto& square : m_board.capturedBlackSquares)
	{
		square.render(m_resources.getWindow(), false);
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

CheckerSquare* CheckersGameState::getClickedSquare()
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
}