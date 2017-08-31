#include "KeyboardAdapter.h"
extern BlockingQueue<std::shared_ptr<IMessage>> bque;
std::map<std::string, uint8_t> npr{
	{ "KEY_UP_ARROW",0xDA },
	{ "KEY_DOWN_ARROW",0xD9 },
	{ "KEY_LEFT_ARROW",0xD8 },
	{ "KEY_RIGHT_ARROW",0xD7 },
	{ "KEY_BACKSPACE",0xB2 },
	{ "KEY_TAB",0xB3 },
	{ "KEY_RETURN",0xB0 },
	{ "KEY_ESC",0xB1 },
	{ "KEY_INSERT",0xD1 },
	{ "KEY_DELETE",0xD4 },
	{ "KEY_PAGE_UP",0xD3 },
	{"KEY_PAGE_DOWN",0x4E},
	{ "KEY_HOME",0xD6 },
	{ "KEY_END",0xD2 },
	{ "KEY_CAPS_LOCK",0xC1 },
	{ "KEY_F1",0xC2 },
	{ "KEY_F2",0xC3 },
	{ "KEY_F3",0xC4 },
	{ "KEY_F4",0xC5 },
	{ "KEY_F5",0xC6 },
	{ "KEY_F6",0xC7 },
	{ "KEY_F7",0xC8 },
	{ "KEY_F8",0xC9 },
	{ "KEY_F9",0xCA },
	{ "KEY_F10",0xCB },
	{ "KEY_F11",0xCC },
	{ "KEY_F12",0xCD },
	{ "KEY_SPACE",0x2C },
	{ "KEY_LSHIFT",0x81 },
	{ "KEY_LCTR",0x80 },
	{ "KEY_LWIN",0x83 },
	{ "KEY_LALT",0x82 },
	{ "KEY_RSHIFT",0x85 },
	{ "KEY_RCTR",0x84 },
	{ "KEY_RWIN",0x87 },
	{ "KEY_RALT",0x86 },
	{"KEY_PAUSE",0x48},
	{"KEY_SCROLL_LOCK",0x47},
	{"KEY_PRT_SCR",0x46}
};

KeyboardAdapter::KeyboardAdapter()
{
	keyReport.id = 1;
}


KeyboardAdapter::~KeyboardAdapter()
{
}

void KeyboardAdapter::keyboardPush(std::string l)
{
	auto k = KeyCodeFromString(l);
	if (isNonPrintable(k)) //non printable
	{
		k = k - 136;
	}
	else if (isAModifier(k)) {	// modifier
		keyReport.modifiers |= (1 << (k - 128));
		k = 0;
	}
	else {	// printable
		k = keymap[k];
		if (isPrintableReachedWithShift(k)) {// character reached with shift
			keyReport.modifiers |= 0x02;	// the left shift modifier
			k = stripModifiers(k); //limit to ascii(get rid of a modifier)
		}
	}
	addToReport(k);
	auto tp = std::make_shared<Report>(keyReport);
	bque.push(tp);
}

void KeyboardAdapter::keyboardWrite(std::string l)
{
	keyboardPush(l);
	keyboardRelease(l);
}

size_t KeyboardAdapter::keyboardRelease(std::string l)
{
	uint8_t k = KeyCodeFromString(l);
	if (isNonPrintable(k))
	{
		k = k - 136;
	}
	else if (isAModifier(k)) {	// modifier
		keyReport.modifiers &= ~(1 << (k - 128));
		k = 0;
	}
	else {	// printable
		k = keymap[k];
		if (!k)
			return 0;
		if (isPrintableReachedWithShift(k)) {// character reached with shift
			keyReport.modifiers &= ~(0x02);	// the left shift modifier
			k = stripModifiers(k); //limit to ascii(get rid of a modifier)
		}
	}
	subtractFromReport(k);
	auto tp = std::make_shared<Report>(keyReport);
	bque.push(tp);
	return k;
}

void KeyboardAdapter::keyboardReleaseAll(std::string l)
{
	keyReport.modifiers = 0;
	for (int i = 0; i<NR_OF_KEYS; ++i)
	{
		keyReport.keys[i] = 0;
	}
	auto tp = std::make_shared<Report>(keyReport);
	bque.push(tp);
}

KeyReport* KeyboardAdapter::addToReport(uint8_t k)
{
	//check if k is in already pressed
	for (int i = 0; i < NR_OF_KEYS; ++i) {
		if (keyReport.keys[i] == k)
			return &keyReport;
	}
	//add to the first empty
	for (int i = 0; i < NR_OF_KEYS; ++i)
	{
		if (keyReport.keys[i] == 0x00) {
			keyReport.keys[i] = k;
			break;
		}
	}
	return &keyReport;
}

KeyReport* KeyboardAdapter::subtractFromReport(uint8_t k)
{
	for (int i = 0; i<NR_OF_KEYS; ++i) {
		if (keyReport.keys[i] == k)
			keyReport.keys[i] = 0x00;
	}
	return &keyReport;
}

uint8_t KeyboardAdapter::isAModifier(uint8_t k)
{
	return k >= 128;
}

uint8_t KeyboardAdapter::isNonPrintable(uint8_t k) const
{
	return k >= 136;
}

uint8_t KeyboardAdapter::isPrintableReachedWithShift(uint8_t k) const
{
	return (k & SHIFT);
}

uint8_t KeyboardAdapter::stripModifiers(uint8_t k) const
{
	uint8_t p = k;
	p &= 0x7F;
	return p;
}
uint8_t KeyboardAdapter::KeyCodeFromString(std::string s) const
{
	uint8_t k;
	if (npr.count(s))
		k = npr[s];
	else {
		std::istringstream str{ s };
		str >> k;
	}
	return k;
}


