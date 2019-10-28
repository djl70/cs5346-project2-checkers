#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

namespace resources
{
	extern const std::unordered_map<std::string, std::string> textureFiles;
	extern std::unordered_map<std::string, sf::Texture> textures;

	extern const std::unordered_map<std::string, std::string> soundFiles;
	extern std::unordered_map<std::string, sf::SoundBuffer> sounds;

	extern sf::RenderWindow* pWindow;

	bool loadResources();
}