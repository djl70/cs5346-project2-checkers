#include "resources.h"

namespace resources
{
	std::unordered_map<std::string, sf::Texture> g_textures;
	std::unordered_map<std::string, sf::Font> g_fonts;

	bool loadAllResources()
	{
		bool success = true;

		sf::Texture texture;
		success &= texture.loadFromFile("res/background.png");
		g_textures.emplace("background", texture);

		sf::Font font;
		success &= font.loadFromFile("res/arial.ttf");
		g_fonts.emplace("arial", font);

		return success;
	}
}