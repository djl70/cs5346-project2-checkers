#pragma once

#include <string>
#include <unordered_map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
	bool setup(sf::RenderWindow* pWindow);

	sf::RenderWindow* getWindow();
	sf::Texture* getTexture(const std::string& name);
	sf::SoundBuffer* getSound(const std::string& name);
	// void playSound(const std::string& name);

private:
	sf::RenderWindow* m_pWindow;
	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
	//std::unordered_map<std::string, sf::Sound> m_sounds;
};