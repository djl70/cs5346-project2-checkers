#pragma once

class BaseState
{
public:
	virtual void enter() = 0;
	virtual BaseState* event() = 0;
	virtual void render() = 0;
	virtual void exit() = 0;
};