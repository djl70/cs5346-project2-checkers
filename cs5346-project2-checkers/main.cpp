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
	const float kScaling = 1.0f; // Controls the scaling of everything. For values other than 1, the graphics will likely be blurry.
	const float kScreenWidth = 1920.0f * kScaling;
	const float kScreenHeight = 1080.0f * kScaling;
	const float kSquareWidth = 100.0f * kScaling;
	const float kCapturedAreaOffsetFromEdge = 200.0f * kScaling;
	const sf::Vector2f boardTopLeft{ (config::kScreenWidth - config::kSquareWidth * 8.0f) * 0.5f, (config::kScreenHeight - config::kSquareWidth * 8.0f) * 0.5f };
	const sf::Vector2f capturedRedTopLeft{ kCapturedAreaOffsetFromEdge, (config::kScreenHeight - config::kSquareWidth * 4.0f) * 0.5f };
	const sf::Vector2f capturedBlackTopLeft{ kScreenWidth - config::kSquareWidth * 3.0f - kCapturedAreaOffsetFromEdge, (config::kScreenHeight - config::kSquareWidth * 4.0f) * 0.5f };
}

int main()
{
	const unsigned int fps = 60;

	sf::RenderWindow window(sf::VideoMode(config::kScreenWidth, config::kScreenHeight), "Checkers", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(fps);

	resources::loadResources();

	sf::Sprite background;
	background.setTexture(resources::textures["background"]);
	background.setScale({ config::kScaling, config::kScaling });

	sf::Sprite sprite;
	sprite.setScale({ config::kScaling, config::kScaling });

	sf::Color colorBrown(143, 67, 15, 200);
	sf::Color colorCream(242, 174, 126, 200);

	sf::RectangleShape rect;
	rect.setSize({ config::kSquareWidth, config::kSquareWidth });

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		window.clear(sf::Color::White);

		window.draw(background);

		// Draw game board
		int i = 0;
		for (int r = 0; r < 8; ++r)
		{
			for (int c = 0; c < 8; ++c)
			{
				sf::Vector2f position{ config::boardTopLeft.x + config::kSquareWidth * c, config::boardTopLeft.y + config::kSquareWidth * r };

				rect.setPosition(position);
				rect.setFillColor((i + r) % 2 == 0 ? colorCream : colorBrown);
				window.draw(rect);

				if (r < 3)
				{
					if (c < 4)
					{
						sprite.setTexture(resources::textures["red_king"]);
						sprite.setPosition(position);
						window.draw(sprite);
					}
					else
					{
						sprite.setTexture(resources::textures["red_man"]);
						sprite.setPosition(position);
						window.draw(sprite);
					}
				}
				else if (r > 4)
				{
					if (c < 4)
					{
						sprite.setTexture(resources::textures["black_king"]);
						sprite.setPosition(position);
						window.draw(sprite);
					}
					else
					{
						sprite.setTexture(resources::textures["black_man"]);
						sprite.setPosition(position);
						window.draw(sprite);
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
					sprite.setTexture(resources::textures["red_king"]);
					sprite.setPosition(redPosition);
					window.draw(sprite);

					sprite.setTexture(resources::textures["black_king"]);
					sprite.setPosition(blackPosition);
					window.draw(sprite);
				}
				else
				{
					sprite.setTexture(resources::textures["red_man"]);
					sprite.setPosition(redPosition);
					window.draw(sprite);

					sprite.setTexture(resources::textures["black_man"]);
					sprite.setPosition(blackPosition);
					window.draw(sprite);
				}
			}
		}

		window.display();
	}

	return 0;
}