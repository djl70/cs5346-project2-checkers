#pragma once

#include "command.h"

#include <cstddef>

struct CheckersGameState;

struct MoveInfo
{
	MoveInfo(CheckersGameState& gameState, std::size_t moveFrom, std::size_t moveTo, std::size_t movedPiece);

	CheckersGameState& gameState;
	std::size_t moveFrom;
	std::size_t moveTo;
	std::size_t movedPiece;
};

class MoveCommand : public Command
{
public:
	MoveCommand(const MoveInfo& info);

	void execute() override;
	void undo() override;

private:
	MoveInfo m_info;
};