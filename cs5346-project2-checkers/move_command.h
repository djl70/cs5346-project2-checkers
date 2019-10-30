#pragma once

#include "command.h"

class CheckerSquare;
class Checkerboard;

struct MoveInfo
{
	MoveInfo(CheckerSquare& from, CheckerSquare& to);
	CheckerSquare& from;
	CheckerSquare& to;
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