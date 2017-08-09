#include "stdafx.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
}
void Keyboard::push(keyType key)
{
	for (keyType &k : keys)
	{
		if (k == key)
			return;
	}
	for (keyType &k : keys)
	{
		if (k == 0x00)
		{
			k = key;
		}
	}
}
void Keyboard::release(keyType key)
{
	for (keyType &k : keys)
	{
		if (k == key)
			k = 0x00;
	}
}
void Keyboard::releaseAll()
{
	for (keyType &k : keys)
		k = 0x00;
}

std::array<keyType, MAX_KEYS> Keyboard::getKeys()
{
	return keys;
}

Keyboard::~Keyboard()
{
}
