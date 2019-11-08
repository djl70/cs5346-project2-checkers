#pragma once

#include "base_state.h"

#include "player.h"

#include <array>

class SimulationState : public BaseState
{
public:
	SimulationState();
	~SimulationState() override;
	void enter() override;
	BaseState* event() override;
	void render() override;

private:
	std::array<Player*, 2> m_players;
	checkerboard::Checkerboard m_board;
};