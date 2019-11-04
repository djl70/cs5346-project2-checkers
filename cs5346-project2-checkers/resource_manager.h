#pragma once

#include <deque>
#include <string>
#include <unordered_map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
	bool loadResources(sf::RenderWindow* pWindow);
	void freeResources();

	sf::RenderWindow* getWindow();
	sf::Texture* getTexture(const std::string& name);
	sf::Font* getFont(const std::string& name);
	void playSound(const std::string& name);

	void update();

private:
	sf::RenderWindow* m_pWindow;
	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unordered_map<std::string, sf::SoundBuffer> m_soundBuffers;
	std::unordered_map<std::string, sf::Font> m_fonts;

	std::deque<sf::Sound> m_playingSounds;
};