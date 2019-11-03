#pragma once

//#include "checkerboard.h"
#include "command.h"

#include <SFML/Graphics.hpp>

namespace checkerboard
{
	struct Checkerboard;
}

struct MoveInfo
{
	MoveInfo(const checkerboard::Checkerboard& board, const sf::Vector2i& from, const sf::Vector2i& to);
	sf::Vector2i from;
	sf::Vector2i to;
	bool promoted;
};

class MoveCommand : public Command
{
public:
	MoveCommand(checkerboard::Checkerboard& board, const MoveInfo& info);

	void execute() override;
	void undo() override;

private:
	checkerboard::Checkerboard& m_board;
	MoveInfo m_info;

	int m_numTurnsSinceCaptureOrKinging;
};