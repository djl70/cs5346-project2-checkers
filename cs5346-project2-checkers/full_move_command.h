#pragma once

#include "command.h"

#include <SFML/Graphics.hpp>

#include <vector>

struct Checkerboard;

struct FullMoveInfo
{
	sf::Vector2i from;
	std::vector<sf::Vector2i> to;
	std::vector<sf::Vector2i> jumped;
	bool promoted;
};

class FullMoveCommand : public Command
{
public:
	FullMoveCommand(Checkerboard& board, const FullMoveInfo& info);

	bool executeStep();
	bool undoStep();

	void execute() override;
	void undo() override;

	bool isJump() const;
	bool didPromote() const;

private:
	Checkerboard& m_board;
	FullMoveInfo m_info;

	int m_firstCapturedIndex;
	int m_partialExecutionStep;
};