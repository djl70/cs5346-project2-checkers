#pragma once

#include "command.h"

class CheckerSquare;
class Checkerboard;

struct JumpInfo
{
	JumpInfo(CheckerSquare& from, CheckerSquare& to, CheckerSquare& jumped);
	CheckerSquare& from;
	CheckerSquare& to;
	CheckerSquare& jumped;
	bool promoted;
};

class JumpCommand : public Command
{
public:
	JumpCommand(Checkerboard& board, const JumpInfo& info);

	void execute() override;
	void undo() override;

private:
	Checkerboard& m_board;
	JumpInfo m_info;
	CheckerSquare* m_capturedSquare;
};