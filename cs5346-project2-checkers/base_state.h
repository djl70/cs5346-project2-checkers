#pragma once

class BaseState
{
public:
	virtual void enter() = 0;
	virtual BaseState* event() = 0;
	virtual void render() = 0;
	// virtual void exit() = 0;
	// desctructor is a C++ native way to implement the part of the pattern
	// that is supposed to be in exit() method 
	virtual ~BaseState() {};
};