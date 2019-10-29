#pragma once

#include "base_state.h"
#include "resource_manager.h"

//class Player;

class PlayerConfigurationMenuState : public BaseState
{
public:
	PlayerConfigurationMenuState(ResourceManager& resources, unsigned int aiPlayerCount);

	void enter() override;
	BaseState* event() override;
	void render() override;
	void exit() override;

private:
	ResourceManager& m_resources;
	unsigned int m_aiPlayerCount;
	//Player* m_pBlackPlayer;
	//Player* m_pRedPlayer;
};