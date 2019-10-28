#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace resources
{
	std::unordered_map<std::string, std::string> textureFiles = {
		{ "background", "res/checkers_game_field.png" },
		{ "red_king", "res/checker_piece_cream_king.png" },
		{ "black_king", "res/checker_piece_brown_king.png" },
		{ "red_man", "res/checker_piece_cream.png" },
		{ "black_man", "res/checker_piece_brown.png" }
	};

	std::unordered_map<std::string, sf::Texture> textures;

	sf::RenderWindow* pWindow;

	bool loadResources()
	{
		bool success;
		sf::Texture texture;

		for (const auto& pair : textureFiles)
		{
			success &= texture.loadFromFile(pair.second);
			textures[pair.first] = texture;
		}

		return success;
	}
}

namespace config
{ 
	const unsigned int kFps = 60;
	const float kScaling = 1.0f; // Controls the scaling of everything. For values other than 1, the graphics will likely be blurry.
	const float kScreenWidth = 1920.0f * kScaling;
	const float kScreenHeight = 1080.0f * kScaling;
	const float kSquareWidth = 100.0f * kScaling;
	const float kCapturedAreaOffsetFromEdge = 200.0f * kScaling;
	const sf::Vector2f boardTopLeft{ (config::kScreenWidth - config::kSquareWidth * 8.0f) * 0.5f, (config::kScreenHeight - config::kSquareWidth * 8.0f) * 0.5f };
	const sf::Vector2f capturedRedTopLeft{ kCapturedAreaOffsetFromEdge, (config::kScreenHeight - config::kSquareWidth * 4.0f) * 0.5f };
	const sf::Vector2f capturedBlackTopLeft{ kScreenWidth - config::kSquareWidth * 3.0f - kCapturedAreaOffsetFromEdge, (config::kScreenHeight - config::kSquareWidth * 4.0f) * 0.5f };
}

enum CheckerColor
{
	kBlack,
	kRed
};

enum CheckerRank
{
	kMan,
	kKing
};

class CheckerPiece
{
public:
	CheckerPiece(CheckerColor color)
		: m_color{ color }
		, m_rank{ kMan }
	{
		m_sprite.setScale({ config::kScaling, config::kScaling });
		updateTexture();
	}

	void promote()
	{
		m_rank = kKing;
		updateTexture();
	}

	void setPosition(const sf::Vector2f& position)
	{
		m_position = position;
		m_sprite.setPosition(position);
	}

	CheckerColor getColor() const
	{
		return m_color;
	}

	bool contains(const sf::Vector2f& point) const
	{
		sf::FloatRect rect({ m_position.x, m_position.y, config::kSquareWidth, config::kSquareWidth });
		return rect.contains(point);
	}

	void render(sf::RenderWindow* pWindow) const
	{
		pWindow->draw(m_sprite);
	}

private:
	CheckerColor m_color;
	CheckerRank m_rank;
	sf::Vector2f m_position;
	sf::Sprite m_sprite;

	void updateTexture()
	{
		switch (m_color)
		{
		case kBlack:
			if (m_rank == kMan) { m_sprite.setTexture(resources::textures["black_man"]); }
			else if (m_rank == kKing) { m_sprite.setTexture(resources::textures["black_king"]); }
			break;
		case kRed:
			if (m_rank == kMan) { m_sprite.setTexture(resources::textures["red_man"]); }
			else if (m_rank == kKing) { m_sprite.setTexture(resources::textures["red_king"]); }
			break;
		}
	}
};

enum CheckerSquareType
{
	kBlackSquare,
	kRedSquare,
	kCapturedBlack,
	kCapturedRed
};

class CheckerSquare
{
public:
	CheckerSquare(CheckerSquareType type)
		: m_type{ type }
		, m_interactable{ type == kBlackSquare }
		, m_piece{ nullptr }
		, m_isKingRow{ false }
	{
		sf::Color fillColor;
		switch (m_type)
		{
		case kBlackSquare:
			fillColor = { 143, 67, 15, 200 };
			break;
		case kRedSquare:
			fillColor = { 242, 174, 126, 200 };
			break;
		default:
			fillColor = sf::Color::Transparent;
			break;
		}
		m_shape.setFillColor(fillColor);
	}

	void setPromotionColor(CheckerColor color)
	{
		m_promoteColor = color;
		m_isKingRow = true;
	}

	void setPosition(const sf::Vector2f& position)
	{
		m_rect = { position.x, position.y, config::kSquareWidth, config::kSquareWidth };
		m_shape.setPosition({ m_rect.left, m_rect.top });
		m_shape.setSize({ m_rect.width, m_rect.height });
	}

	void setPiece(CheckerPiece* piece)
	{
		m_piece = piece;
		if (m_piece)
		{
			m_piece->setPosition({ m_rect.left, m_rect.top });
			if (m_isKingRow && m_promoteColor == m_piece->getColor())
			{
				m_piece->promote();
			}
		}
	}

	CheckerPiece* getPiece()
	{
		return m_piece;
	}

	bool contains(const sf::Vector2f& point) const
	{
		return m_interactable && m_rect.contains(point);
	}

	void render(sf::RenderWindow* pWindow) const
	{
		pWindow->draw(m_shape);
		if (m_piece)
		{
			m_piece->render(pWindow);
		}
	}

private:
	CheckerSquareType m_type;
	sf::FloatRect m_rect;
	bool m_interactable;
	CheckerPiece* m_piece;
	bool m_isKingRow;
	CheckerColor m_promoteColor;

	sf::RectangleShape m_shape;
};

class BaseState
{
public:
	virtual void enter() = 0;
	virtual BaseState* event() = 0;
	virtual void render() = 0;
	virtual void exit() = 0;
};

class CheckersGameState : public BaseState
{
public:
	void enter() override
	{
		resources::loadResources();

		m_background.setTexture(resources::textures["background"]);
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

		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 3; ++c)
			{
				sf::Vector2f redPosition{ config::capturedRedTopLeft.x + config::kSquareWidth * c, config::capturedRedTopLeft.y + config::kSquareWidth * r };
				CheckerSquare redSquare(kCapturedRed);
				redSquare.setPosition(redPosition);
				m_capturedRedSquares.push_back(redSquare);

				sf::Vector2f blackPosition{ config::capturedBlackTopLeft.x + config::kSquareWidth * c, config::capturedBlackTopLeft.y + config::kSquareWidth * r };
				CheckerSquare blackSquare(kCapturedBlack);
				blackSquare.setPosition(blackPosition);
				m_capturedBlackSquares.push_back(blackSquare);
			}
		}

		m_selectionProgress = kNoCheckerSelected;
		m_selectedPiece = -1;
	}

	BaseState* event() override
	{
		sf::Event event;
		while (resources::pWindow->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				resources::pWindow->close();
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
								if (m_capturedBlackSquares[i].getPiece() == nullptr)
								{
									m_capturedBlackSquares[i].setPiece(piece);
									break;
								}
							}
							break;
						case kRed:
							for (int i = 0; i < m_capturedRedSquares.size(); ++i)
							{
								if (m_capturedRedSquares[i].getPiece() == nullptr)
								{
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

		return nullptr;
	}

	void render() override
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

	void exit() override
	{

	}

private:
	sf::Sprite m_background;
	sf::Sprite m_sprite;
	sf::Color m_blackPieceColor;
	sf::Color m_redPieceColor;
	sf::RectangleShape m_rect;
	std::vector<CheckerPiece> m_pieces;
	std::vector<CheckerSquare> m_squares;
	std::vector<CheckerSquare> m_capturedRedSquares;
	std::vector<CheckerSquare> m_capturedBlackSquares;

	enum CheckerSelectionProgress
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
	int m_selectedPiece;
};

class CheckersGameEngine
{
public:
	CheckersGameEngine(unsigned int fps)
		: m_window(sf::VideoMode(config::kScreenWidth, config::kScreenHeight), "Checkers", sf::Style::Titlebar | sf::Style::Close)
		, m_pState{ nullptr }
	{
		m_window.setFramerateLimit(fps);
		resources::pWindow = &m_window;

		m_pState = new CheckersGameState;
		m_pState->enter();
	}

	~CheckersGameEngine()
	{
		if (m_pState)
		{
			m_pState->exit();
			delete m_pState;
			m_pState = nullptr;
		}
	}

	void run()
	{
		while (resources::pWindow->isOpen())
		{
			BaseState* pNextState = m_pState->event();
			m_pState->render();

			if (pNextState)
			{
				m_pState->exit();
				delete m_pState;
				m_pState = pNextState;
				m_pState->enter();
			}
		}

		if (m_pState)
		{
			m_pState->exit();
			delete m_pState;
			m_pState = nullptr;
		}
	}

private:
	sf::RenderWindow m_window;
	BaseState* m_pState;
};

int main()
{
	CheckersGameEngine game(config::kFps);
	game.run();

	return 0;
}