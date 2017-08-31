#include "stdafx.h"
#include "KeyboardTranslator.h"
#include <stdexcept>


KeyboardTranslator::KeyboardTranslator()
{
	FillModMap();
}
keyType KeyboardTranslator::Translate(keyType key)
{
	keyType retval;
		try {	
			retval=modifierMap.at(key);		}
		catch (const std::out_of_range &ex)
		{
			retval = key;
		}
		return retval;
}


KeyboardTranslator::~KeyboardTranslator()
{
}

void KeyboardTranslator::FillModMap()
{
	modifierMap[0xA2] = 0xE0;//lctrl
	modifierMap[0xA0] = 0xE1;//lshift
	modifierMap[0xA4] = 0xE2;//lalt
	modifierMap[0xA3] = 0xE0;//rctrl
	modifierMap[0xA1] = 0xE1;//rshift
	modifierMap[0xA5] = 0xE2;//ralt
	modifierMap[0x5B] = 0xE3;//lwin
	modifierMap[0x5C] = 0xE7;//rwin



	/*
	#define KEY_LEFT_CTRL   0x80
	#define KEY_LEFT_SHIFT    0x81
	#define KEY_LEFT_ALT    0x82
	#define KEY_LEFT_GUI    0x83
	#define KEY_RIGHT_CTRL    0x84
	#define KEY_RIGHT_SHIFT   0x85
	#define KEY_RIGHT_ALT   0x86
	#define KEY_RIGHT_GUI   0x87
	///
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
		modifierMap[i] = i +0x52-136;
	}
	/*
	#define KEY_UP_ARROW    0xDA
	#define KEY_DOWN_ARROW    0xD9
	#define KEY_LEFT_ARROW    0xD8
	#define KEY_RIGHT_ARROW   0xD7

	*/
	modifierMap[0x25] = 0xD8-136;
	modifierMap[0x26] = 0xDA-136;
	modifierMap[0x27] = 0xD7-136;
	modifierMap[0x28] = 0xD9-136;
	
	modifierMap[0x21] = 0xD3-136;//pup
	modifierMap[0x22] = 0xD6-136;//pdw
	modifierMap[0x23] = 0xD5-136;//end
	modifierMap[0x24] = 0xD2-136;//home
	modifierMap[0x2D] = 0xD1-136;//insert
	modifierMap[0x2E] = 0xD4-136;//delete
	modifierMap[0x14] = 0xC1-136;//caps
	modifierMap[0x1B] = 0xB1-136;//esc


	//numbers
	modifierMap[0x30] = 0x27;
	for (unsigned int i = 0x31; i <= 0x39; ++i)
	{
		modifierMap[i] = i - 0x13;
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
	modifierMap[0x08] = 0xB2-136;
	modifierMap[0x09] = 0xB3-136;
	modifierMap[0x0D] = 0xB0-136;
}
