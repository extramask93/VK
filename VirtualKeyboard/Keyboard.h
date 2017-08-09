#pragma once
#include <cstdint>
class Keyboard
{
public:

	Keyboard();
	void push(uint8_t key);
	void release(uint8_t key);
	void releaseAll();
	~Keyboard();
private:
	static const uint8_t id = 1;

};

