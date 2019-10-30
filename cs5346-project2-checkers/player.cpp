#include "player.h"

#include "checkerboard.h"

Player::Player(CheckerColor color)
	: m_isTurn{ false }
	, m_color{ color }
{

}

void Player::setBoard(Checkerboard* pBoard)
{
	m_pBoard = pBoard;
}

void Player::setResources(ResourceManager* pResources)
{
	m_pResources = pResources;
}

void Player::takeTurn()
{
	m_isTurn = true;
}

bool Player::isTurn() const
{
	return m_isTurn;
}