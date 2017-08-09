#pragma once
#include <stdint.h>
#include <array>
#include <boost\signals2.hpp>
#define MAX_KEYS 6
typedef uint32_t keyType;
class Keyboard
{
public:
	boost::singal2<void(keyType)> sgn;
	Keyboard();
	void push(keyType key);
	void release(keyType key);
	void releaseAll();
	std::array<keyType, MAX_KEYS> getKeys();
	~Keyboard();
private:
	const keyType id = 1;
	std::array<keyType,MAX_KEYS> keys;
};

