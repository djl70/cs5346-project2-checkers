#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

namespace resources
{
	bool loadAllResources();

	extern std::unordered_map<std::string, sf::Texture> g_textures;
	extern std::unordered_map<std::string, sf::Font> g_fonts;
}