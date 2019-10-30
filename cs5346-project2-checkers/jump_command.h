#pragma once

#include "command.h"

#include <SFML/Graphics.hpp>

class CheckerSquare;
class Checkerboard;

struct JumpInfo
{
	JumpInfo(CheckerSquare& from, CheckerSquare& to, CheckerSquare& jumped);
	sf::Vector2i from;
	sf::Vector2i to;
	sf::Vector2i jumped;
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