#include "stdafx.h"
#include "KeyboardTranslator.h"
#include <stdexcept>


KeyboardTranslator::KeyboardTranslator()
{
	fillModMap();
}
keyType KeyboardTranslator::translate(keyType key)
{
	keyType retval;
		try {
			retval=modifierMap.at(key);
			printf("clicked: %i\n", retval);
		}
		catch (const std::out_of_range &ex)
		{
			retval = 0;
		}
		return retval;
}
/*
bool KeyboardTranslator::isModifier(keyType key)
{
	if ((key >= 0x10 && key <= 0x12) || key == 0x14 || key == 0x5B || key == 0x5C)
		return true;
	else
		return false;
}

bool KeyboardTranslator::isASCII(keyType key)
{
	if (key >= 0x30 && key <= 0x5A)
		return true;
	else
		return false;
}

bool KeyboardTranslator::isFunctional(keyType key)
{
	if (key >= 0x70 && key <= 0x7B) //f1-f12
		return true;
	if (key >= 0x60 && key <= 0x6F) //numpad
		return true;
	if (key == 0x08 || key <= 0x20 || key == 0x09 || key == 0x0D || key == 0x13 || key == 0x1B)
		return true;
	if (key >= 0x20 && key <= 0x2F)
		return true;
	return false;
}*/



KeyboardTranslator::~KeyboardTranslator()
{
}

void KeyboardTranslator::fillModMap()
{
	modifierMap[(keyType)0x10] = (uint8_t)0x81;//shift
	modifierMap[0x11] = 0x80;//ctrl
	modifierMap[0x12] = 0x82;//alt
	modifierMap[0x5B] = 0x83;//gui
	modifierMap[0x5B] = 0x83;//gui

	/*
	#define KEY_F1        0xC2
	#define KEY_F2        0xC3
	#define KEY_F3        0xC4
	#define KEY_F4        0xC5
	#define KEY_F5        0xC6
	#define KEY_F6        0xC7
	#define KEY_F7        0xC8
	#define KEY_F8        0xC9
	#define KEY_F9        0xCA
	#define KEY_F10       0xCB
	#define KEY_F11       0xCC
	#define KEY_F12       0xCD
	*/
	for (unsigned int i = 0x70; i <= 0x7B; ++i)
	{
		modifierMap[i] = i +0x52;
	}
	/*
	#define KEY_UP_ARROW    0xDA
	#define KEY_DOWN_ARROW    0xD9
	#define KEY_LEFT_ARROW    0xD8
	#define KEY_RIGHT_ARROW   0xD7

	*/
	modifierMap[0x25] = 0xD8;
	modifierMap[0x26] = 0xDA;
	modifierMap[0x27] = 0xD7;
	modifierMap[0x28] = 0xD9;
	

	//numbers
	for (unsigned int i = 0x30; i <= 0x39; ++i)
	{
		modifierMap[i] = i - 0x09;
	}
	//A-Z
	for (unsigned int i = 0x41; i <= 0x5A; ++i)
	{
		modifierMap[i] = i - 0x3D;
	}
	/*
	0x36,          // ,
	0x2d,          // -
	0x37,          // .
	0x38,          // /
	*/
	modifierMap[0xBC] = 0x36;
	modifierMap[0xBD] = 0x2d;
	modifierMap[0xBE] = 0x37;
	modifierMap[0xBF] = 0x38;
	/*
	0x33,          // ;
	0x2e,          // =
	0x34,          // '
	0x2c,		   //  ' '
	0x2f,          // [
	0x31,          // bslash
	0x30,          // ]
	0x35,          // `
	*/
	modifierMap[0xBA] = 0x33;
	modifierMap[0xBB] = 0x23;
	modifierMap[0xDE] = 0x34;
	modifierMap[0x20] = 0x2C;
	modifierMap[0xDB] = 0x2F;
	modifierMap[0xDC] = 0x31;
	modifierMap[0xDD] = 0x30;
	modifierMap[0xC0] = 0x35;
	/*
	#define KEY_BACKSPACE   0xB2
	#define KEY_TAB       0xB3
	#define KEY_RETURN      0xB0

	*/
	modifierMap[0x08] = 0xB2;
	modifierMap[0x09] = 0xB3;
	modifierMap[0x0D] = 0xB0;
}
