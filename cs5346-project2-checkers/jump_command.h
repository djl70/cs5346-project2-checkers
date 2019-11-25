#pragma once

#include "command.h"

#include <SFML/Graphics.hpp>

namespace checkerboard
{
	struct Checkerboard;
}

struct JumpInfo
{
	JumpInfo(const checkerboard::Checkerboard& board, const sf::Vector2i& from, const sf::Vector2i& to, const sf::Vector2i& jumped);
	sf::Vector2i from;
	sf::Vector2i to;
	sf::Vector2i jumped;
	bool promoted;
};

class JumpCommand : public Command
{
public:
	JumpCommand(checkerboard::Checkerboard& board, const JumpInfo& info);

	void execute() override;
	void undo() override;

private:
	checkerboard::Checkerboard& m_board;
	JumpInfo m_info;
	int m_capturedSquare;

	int m_numTurnsSinceCaptureOrKinging;
};