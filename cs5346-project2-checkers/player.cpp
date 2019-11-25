#include "player.h"

Player::Player(CheckerColor color, bool isBot)
	: m_color{ color }
	, m_isBot{ isBot }
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

CheckerColor Player::getColor() const
{
	return m_color;
}