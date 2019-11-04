#include "resource_manager.h"

#include "config.h"

bool ResourceManager::loadResources(sf::RenderWindow* pWindow)
{
	m_pWindow = pWindow;

	bool success;

	for (const auto& pair : config::textureFiles)
	{
		success &= m_textures[pair.first].loadFromFile(pair.second);
	}
	for (const auto& pair : config::fontFiles)
	{
		success &= m_fonts[pair.first].loadFromFile(pair.second);
	}
	for (const auto& pair : config::soundFiles)
	{
		success &= m_soundBuffers[pair.first].loadFromFile(pair.second);
	}

	return success;
}

void ResourceManager::freeResources()
{
	// Stop and remove all queued sounds
	while (!m_playingSounds.empty())
	{
		m_playingSounds.front().stop();
		m_playingSounds.front().resetBuffer();
		m_playingSounds.pop_front();
	}
}

sf::RenderWindow* ResourceManager::getWindow()
{
	return m_pWindow;
}

sf::Texture* ResourceManager::getTexture(const std::string& name)
{
	return &m_textures.at(name);
}

sf::Font* ResourceManager::getFont(const std::string& name)
{
	return &m_fonts.at(name);
}

void ResourceManager::playSound(const std::string& name)
{
	// Enqueue and play
	m_playingSounds.emplace_back(m_soundBuffers.at(name));
	m_playingSounds.back().play();
}

void ResourceManager::update()
{
	// Check for sounds that are finished playing and dequeue them
	while (!m_playingSounds.empty() && m_playingSounds.front().getStatus() == sf::Sound::Status::Stopped)
	{
		m_playingSounds.front().stop();
		m_playingSounds.front().resetBuffer();
		m_playingSounds.pop_front();
	}
}