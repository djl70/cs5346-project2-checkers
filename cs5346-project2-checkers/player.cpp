#include "player.h"

Player::Player(CheckerColor color, bool isBot)
	: m_color{ color }
	, m_isBot{ isBot }
	, m_isTurn{ false }
{

}

void Player::setBoard(checkerboard::Checkerboard* pBoard)
{
	m_pBoard = pBoard;
}

void Player::setResources(ResourceManager* pResources)
{
	m_pResources = pResources;
}

bool Player::isBot() const
{
	return m_isBot;
}

bool Player::isTurn() const
{
	return m_isTurn;
}

CheckerColor Player::getColor() const
{
	return m_color;
}

void Player::takeTurn()
{
	m_isTurn = true;
}