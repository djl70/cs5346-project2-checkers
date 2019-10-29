#include "player_configuration_menu_state.h"

PlayerConfigurationMenuState::PlayerConfigurationMenuState(ResourceManager& resources, unsigned int aiPlayerCount)
	: m_resources{ resources }
	, m_aiPlayerCount{ aiPlayerCount }
	//, m_pBlackPlayer{ nullptr }
	//, m_pRedPlayer{ nullptr }
{

}

void PlayerConfigurationMenuState::enter()
{
	// TODO: Set up GUI with buttons.
	// If aiPlayerCount == 0, return immediately.
	// If aiPlayerCount >= 1, enable buttons on the left side
	// If aiPlayerCount == 2, enable buttons on the right side
}

BaseState* PlayerConfigurationMenuState::event()
{
	sf::Vector2f mousePositionInWindow = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_resources.getWindow()));

	return nullptr;
}

void PlayerConfigurationMenuState::render()
{

}

void PlayerConfigurationMenuState::exit()
{

}