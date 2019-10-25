#pragma once

#include "command.h"

class CheckerPiece;
class CheckerSquare;

struct MoveInfo
{
	MoveInfo(CheckerSquare& from, CheckerSquare& to, CheckerPiece& movedPiece)
		: from{ from }
		, to{ to }
		, movedPiece{ movedPiece }
	{

	}

	CheckerSquare& from;
	CheckerSquare& to;
	CheckerPiece& movedPiece;
};

class MoveCommand : public Command
{
public:
	MoveCommand(const MoveInfo& info)
		: m_info{ info }
	{

	}

	void execute() override
	{

	}

	void undo() override
	{

	}

private:
	MoveInfo m_info;
};