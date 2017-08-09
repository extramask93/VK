#pragma once
#include "Keyboard.h"
class Mouse
{
public:
	Mouse();
	void push(keyType key);
	void release(keyType key);
	void releaseAll();
	void updatePosition(int x, int y);
	void updateWheel(int distance);
	~Mouse();
};

