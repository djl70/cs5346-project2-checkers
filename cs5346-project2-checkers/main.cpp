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

		m_sprite.setScale({ config::kScaling, config::kScaling });

		m_blackPieceColor = { 143, 67, 15, 200 };
		m_redPieceColor = { 242, 174, 126, 200 };

		m_rect.setSize({ config::kSquareWidth, config::kSquareWidth });
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
			}
		}

		return nullptr;
	}

	void render() override
	{
		resources::pWindow->clear(sf::Color::White);

		resources::pWindow->draw(m_background);

		// Draw game board
		int i = 0;
		for (int r = 0; r < 8; ++r)
		{
			for (int c = 0; c < 8; ++c)
			{
				sf::Vector2f position{ config::boardTopLeft.x + config::kSquareWidth * c, config::boardTopLeft.y + config::kSquareWidth * r };

				m_rect.setPosition(position);
				m_rect.setFillColor((i + r) % 2 == 0 ? m_redPieceColor : m_blackPieceColor);
				resources::pWindow->draw(m_rect);

				if (r < 3)
				{
					if (c < 4)
					{
						m_sprite.setTexture(resources::textures["red_king"]);
						m_sprite.setPosition(position);
						resources::pWindow->draw(m_sprite);
					}
					else
					{
						m_sprite.setTexture(resources::textures["red_man"]);
						m_sprite.setPosition(position);
						resources::pWindow->draw(m_sprite);
					}
				}
				else if (r > 4)
				{
					if (c < 4)
					{
						m_sprite.setTexture(resources::textures["black_king"]);
						m_sprite.setPosition(position);
						resources::pWindow->draw(m_sprite);
					}
					else
					{
						m_sprite.setTexture(resources::textures["black_man"]);
						m_sprite.setPosition(position);
						resources::pWindow->draw(m_sprite);
					}
				}

				++i;
			}
		}

		// Draw captured pieces
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 3; ++c)
			{
				sf::Vector2f redPosition{ config::capturedRedTopLeft.x + config::kSquareWidth * c, config::capturedRedTopLeft.y + config::kSquareWidth * r };
				sf::Vector2f blackPosition{ config::capturedBlackTopLeft.x + config::kSquareWidth * c, config::capturedBlackTopLeft.y + config::kSquareWidth * r };

				if (r < 2)
				{
					m_sprite.setTexture(resources::textures["red_king"]);
					m_sprite.setPosition(redPosition);
					resources::pWindow->draw(m_sprite);

					m_sprite.setTexture(resources::textures["black_king"]);
					m_sprite.setPosition(blackPosition);
					resources::pWindow->draw(m_sprite);
				}
				else
				{
					m_sprite.setTexture(resources::textures["red_man"]);
					m_sprite.setPosition(redPosition);
					resources::pWindow->draw(m_sprite);

					m_sprite.setTexture(resources::textures["black_man"]);
					m_sprite.setPosition(blackPosition);
					resources::pWindow->draw(m_sprite);
				}
			}
		}

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