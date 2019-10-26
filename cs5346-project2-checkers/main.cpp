#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace resources
{
	std::unordered_map<std::string, std::string> textureFiles = {
		{ "background", "res/background.png" },
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
	const float kScreenWidth = 1920.0f;
	const float kScreenHeight = 1080.0f;
	const float kSquareWidth = 100.0f;
}

int main()
{
	const unsigned int fps = 60;

	sf::RenderWindow window(sf::VideoMode(config::kScreenWidth, config::kScreenHeight), "Checkers", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(fps);

	resources::loadResources();

	sf::Sprite background;
	background.setTexture(resources::textures["background"]);

	sf::Sprite sprite;

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

		sf::Vector2f topLeft{ (config::kScreenWidth - config::kSquareWidth * 8.0f) * 0.5f, (config::kScreenHeight - config::kSquareWidth * 8.0f) * 0.5f };
		int i = 0;
		for (int r = 0; r < 8; ++r)
		{
			for (int c = 0; c < 8; ++c)
			{
				sf::Vector2f position{ topLeft.x + config::kSquareWidth * c, topLeft.y + config::kSquareWidth * r };

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

		window.display();
	}

	return 0;
}