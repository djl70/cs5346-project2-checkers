#include "move_command.h"

#include "checker_piece.h"
#include "checker_square.h"
#include "checkers_game_state.h"

MoveInfo::MoveInfo(CheckersGameState& gameState, std::size_t moveFrom, std::size_t moveTo, std::size_t movedPiece)
	: gameState{ gameState }
	, moveFrom{ moveFrom }
	, moveTo{ moveTo }
	, movedPiece{ movedPiece }
{

}

MoveCommand::MoveCommand(const MoveInfo& info)
	: m_info{ info }
{

}

void MoveCommand::execute()
{
	movePiece(*m_info.gameState.squares[m_info.moveFrom], *m_info.gameState.squares[m_info.moveTo]);
}

void MoveCommand::undo()
{

}