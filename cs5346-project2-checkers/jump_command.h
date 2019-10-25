#pragma once

#include "command.h"

class CheckerPiece;
class CheckerSquare;

struct JumpInfo
{
	JumpInfo(CheckerSquare& from, CheckerSquare& to, CheckerPiece& movedPiece, CheckerPiece& jumpedPiece)
		: from{ from }
		, to{ to }
		, movedPiece{ movedPiece }
		, jumpedPiece{ jumpedPiece }
	{

	}

	CheckerSquare& from;
	CheckerSquare& to;
	CheckerPiece& movedPiece;
	CheckerPiece& jumpedPiece;
};

class JumpCommand : public Command
{
public:
	JumpCommand(const JumpInfo& info)
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
	JumpInfo m_info;
};