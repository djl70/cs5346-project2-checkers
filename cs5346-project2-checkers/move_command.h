#pragma once

#include "command.h"

#include <SFML/Graphics.hpp>

class CheckerSquare;
class Checkerboard;

struct MoveInfo
{
	MoveInfo(CheckerSquare& from, CheckerSquare& to);
	sf::Vector2i from;
	sf::Vector2i to;
	bool promoted;
};

class MoveCommand : public Command
{
public:
	MoveCommand(Checkerboard& board, const MoveInfo& info);

	void execute() override;
	void undo() override;

private:
	Checkerboard& m_board;
	MoveInfo m_info;
};