#include "stdafx.h"
#include "Macro.h"
#define SHIFT 0x80
#define KSIZE 128
uint8_t const keymap[KSIZE] = {
	0x00,             // NUL
	0x00,             // SOH
	0x00,             // STX
	0x00,             // ETX
	0x00,             // EOT
	0x00,             // ENQ
	0x00,             // ACK
	0x00,             // BEL
	0x2a,			// BS	Backspace
	0x2b,			// TAB	Tab
	0x28,			// LF	Enter
	0x00,             // VT
	0x00,             // FF
	0x00,             // CR
	0x00,             // SO
	0x00,             // SI
	0x00,             // DEL
	0x00,             // DC1
	0x00,             // DC2
	0x00,             // DC3
	0x00,             // DC4
	0x00,             // NAK
	0x00,             // SYN
	0x00,             // ETB
	0x00,             // CAN
	0x00,             // EM
	0x00,             // SUB
	0x00,             // ESC
	0x00,             // FS
	0x00,             // GS
	0x00,             // RS
	0x00,             // US

	0x2c,		   //  ' '
	0x1e | SHIFT,	   // !
	0x34 | SHIFT,	   // "
	0x20 | SHIFT,    // #
	0x21 | SHIFT,    // $
	0x22 | SHIFT,    // %
	0x24 | SHIFT,    // &
	0x34,          // '
	0x26 | SHIFT,    // (
	0x27 | SHIFT,    // )
	0x25 | SHIFT,    // *
	0x2e | SHIFT,    // +
	0x36,          // ,
	0x2d,          // -
	0x37,          // .
	0x38,          // /
	0x27,          // 0
	0x1e,          // 1
	0x1f,          // 2
	0x20,          // 3
	0x21,          // 4
	0x22,          // 5
	0x23,          // 6
	0x24,          // 7
	0x25,          // 8
	0x26,          // 9
	0x33 | SHIFT,      // :
	0x33,          // ;
	0x36 | SHIFT,      // <
	0x2e,          // =
	0x37 | SHIFT,      // >
	0x38 | SHIFT,      // ?
	0x1f | SHIFT,      // @
	0x04 | SHIFT,      // A
	0x05 | SHIFT,      // B
	0x06 | SHIFT,      // C
	0x07 | SHIFT,      // D
	0x08 | SHIFT,      // E
	0x09 | SHIFT,      // F
	0x0a | SHIFT,      // G
	0x0b | SHIFT,      // H
	0x0c | SHIFT,      // I
	0x0d | SHIFT,      // J
	0x0e | SHIFT,      // K
	0x0f | SHIFT,      // L
	0x10 | SHIFT,      // M
	0x11 | SHIFT,      // N
	0x12 | SHIFT,      // O
	0x13 | SHIFT,      // P
	0x14 | SHIFT,      // Q
	0x15 | SHIFT,      // R
	0x16 | SHIFT,      // S
	0x17 | SHIFT,      // T
	0x18 | SHIFT,      // U
	0x19 | SHIFT,      // V
	0x1a | SHIFT,      // W
	0x1b | SHIFT,      // X
	0x1c | SHIFT,      // Y
	0x1d | SHIFT,      // Z
	0x2f,          // [
	0x31,          // bslash
	0x30,          // ]
	0x23 | SHIFT,    // ^
	0x2d | SHIFT,    // _
	0x35,          // `
	0x04,          // a
	0x05,          // b
	0x06,          // c
	0x07,          // d
	0x08,          // e
	0x09,          // f
	0x0a,          // g
	0x0b,          // h
	0x0c,          // i
	0x0d,          // j
	0x0e,          // k
	0x0f,          // l
	0x10,          // m
	0x11,          // n
	0x12,          // o
	0x13,          // p
	0x14,          // q
	0x15,          // r
	0x16,          // s
	0x17,          // t
	0x18,          // u
	0x19,          // v
	0x1a,          // w
	0x1b,          // x
	0x1c,          // y
	0x1d,          // z
	0x2f | SHIFT,    // {
	0x31 | SHIFT,    // |
	0x30 | SHIFT,    // }
	0x35 | SHIFT,    // ~
	0	// DEL
};

Macro::Macro(BlockingQueue<std::shared_ptr<IMessage>> &que):bque{que}
{
	keyReport.id = 1;
	mouseReport.id = 2;
	mp["delayS"] = std::bind(&Macro::sleepS, this, std::placeholders::_1); //&Macro::sleepS;
	mp["delayMS"] = std::bind(&Macro::sleepMS, this, std::placeholders::_1);
	mp["keyboardPush"] = std::bind(&Macro::keyboardPush, this, std::placeholders::_1);
	mp["keyboardRelease"] = std::bind(&Macro::keyboardRelease, this, std::placeholders::_1);
	mp["keyboardReleaseAll"] = std::bind(&Macro::keyboardReleaseAll, this, std::placeholders::_1);
	mp["mouseMove"] = std::bind(&Macro::mouseMove, this, std::placeholders::_1);
	mp["mouseReleaseAll"] = std::bind(&Macro::mouseReleaseAll, this, std::placeholders::_1);
	mp["mouseRelease"] = std::bind(&Macro::mouseRelease, this, std::placeholders::_1);
	mp["mousePush"] = std::bind(&Macro::mousePush, this, std::placeholders::_1);
}

int Macro::load(std::string const fname)
{
	std::fstream file;
	file.open(fname,std::fstream::in);
	std::string buffer;
	int lnr=1;
	while (std::getline(file,buffer))
	{
		lines.push_back(buffer);
		try{
			std::istringstream temp{ buffer };
			std::string foo;
			temp>> foo;
			mp.at(foo);
		}
		catch (std::exception &ex)
		{
			std::string t = "Unknown command: " + boost::lexical_cast<std::string>(lnr) + buffer;
			throw(std::exception{ t.c_str() });
		}
		lnr++;
	}
	return 0;
}

void Macro::sleepS(std::string l)
{
	std::istringstream str{ l };
	unsigned int time;
	str >> time;
	boost::this_thread::sleep(boost::posix_time::seconds(time));
}

void Macro::sleepMS(std::string l)
{
	std::istringstream str{ l };
	unsigned int time;
	str >> time;
	boost::this_thread::sleep(boost::posix_time::millisec(time));
}

void Macro::keyboardPush(std::string l)
{
	std::istringstream str{ l };
	uint8_t k;
	str >> k;
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
size_t Macro::keyboardRelease(std::string l) {
	std::istringstream str{ l };
	uint8_t k;
	str >> k;
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


void Macro::keyboardReleaseAll(std::string l)
{
		keyReport.modifiers = 0;
		for (int i = 0; i<NR_OF_KEYS; ++i)
		{
			keyReport.keys[i] = 0;
		}
		auto tp = std::make_shared<Report>(keyReport);
		bque.push(tp);
}

size_t Macro::mousePush(std::string l)
{
		std::istringstream str{ l };
		unsigned int key;
		str >> key;
		mouseReport.buttons |= key;
		mouseMove("0,0,0");
		return 0;
}

void Macro::mouseReleaseAll(std::string l)
{

	mouseReport.buttons = 0;
	mouseMove("0,0,0");

}

size_t Macro::mouseRelease(std::string l)
{
		std::istringstream str{ l };
		unsigned int key;
		str >> key;
		mouseReport.buttons &= ~key;
		mouseMove("0,0,0");
		return key;
}

void Macro::mouseMove(std::string l)
{
		std::istringstream str{ l };
		unsigned int X,Y,angle;
		str >> X>>Y>>angle;
		mouseReport.X = X;
		mouseReport.Y = Y;
		mouseReport.Wheel = angle;
		auto tp = std::make_shared<MouseReport>(mouseReport);
		bque.push(tp);
}

Macro::~Macro()
{
}


uint8_t Macro::isAModifier(uint8_t k) {
	return k >= 128;
}
uint8_t Macro::isNonPrintable(uint8_t k) {
	return k >= 136;
}
uint8_t Macro::isPrintableReachedWithShift(uint8_t k) {
	return (k & SHIFT);
}
uint8_t Macro::stripModifiers(uint8_t k)
{
	uint8_t p = k;
	p &= 0x7F;
	return p;
}
KeyReport *Macro::addToReport(uint8_t k)
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
KeyReport* Macro::subtractFromReport(uint8_t k) {
	for (int i = 0; i<NR_OF_KEYS; ++i) {
		if (keyReport.keys[i] == k)
			keyReport.keys[i] = 0x00;
	}
	return &keyReport;
}

void Macro::Run()
{

	for (auto &line : lines)
	{
		std::string func;
		std::string params;
		std::istringstream lstr{ line };
		lstr >> func >> params;
		std::function<void(std::string)> foo;
		try {
			foo = mp.at(func);
		}
		catch (std::out_of_range &ex)
		{
			std::cerr << "Unknown command: " << line << std::endl;;
			return;
		}
		foo(params);
	}
}
