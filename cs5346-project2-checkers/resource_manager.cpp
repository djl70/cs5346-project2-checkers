#include "resource_manager.h"

#include "config.h"

bool ResourceManager::setup(sf::RenderWindow* pWindow)
{
	m_pWindow = pWindow;

	bool success;

	for (const auto& pair : config::textureFiles)
	{
		success &= m_textures[pair.first].loadFromFile(pair.second);
	}
	for (const auto& pair : config::soundFiles)
	{
		success &= m_soundBuffers[pair.first].loadFromFile(pair.second);
		// m_sounds[pair.first].setBuffer(m_soundBuffers[pair.first]);
	}

	return success;
}

sf::RenderWindow* ResourceManager::getWindow()
{
	return m_pWindow;
}

sf::Texture* ResourceManager::getTexture(const std::string& name)
{
	return &m_textures.at(name);
}
sf::SoundBuffer* ResourceManager::getSound(const std::string& name)
{
	return &m_soundBuffers.at(name);
}
/*void ResourceManager::playSound(const std::string& name)
{
	m_sounds.at(name).play();
}*/