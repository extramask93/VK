

#include "stdafx.h"
#include "Keyboard.h"


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
			auto temp=translator.translate(k);
			keyboard.push(temp);
			break;
		}
	}
}
void Keyboard::release(keyType key)
{
	for (keyType &k : keys)
	{
		if (k == key) {
			k = 0x00;
			keyboard.release(translator.translate(key));
		}
	}
}
void Keyboard::releaseAll()
{
	for (keyType &k : keys)
		k = 0x00;
	keyboard.releaseAll();
}

std::array<keyType, MAX_KEYS> Keyboard::getKeys()
{
	return keys;
}


Keyboard::~Keyboard()
{
}
