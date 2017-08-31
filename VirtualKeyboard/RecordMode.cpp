#include "RecordMode.h"
#include <fstream>
#include "HIDKeyboard.h"

extern Mouse mouse;
extern Keyboard keyboard;

namespace MP
{
	std::map<uint8_t, std::string> npp{
		{ 0x26,"KEY_UP_ARROW" },{ 0x28,"KEY_DOWN_ARROW" },{ 0x25,"KEY_LEFT_ARROW" },{ 0x27,"KEY_RIGHT_ARROW" },
		{ 0x08,"KEY_BACKSPACE" },{ 0x09,"KEY_TAB" },{ 0x0D,"KEY_RETURN" },{ 0x1B,"KEY_ESC" },{ 0x2D,"KEY_INSERT" },
		{ 0x2E,"KEY_DELETE" },{ 0x21,"KEY_PAGE_UP" },{ 0x22,"KEY_PAGE_DOWN" },{ 0x24,"KEY_HOME" },{ 0x23,"KEY_END" },
		{0x13,"KEY_PAUSE"},{0x91,"KEY_SCROLL_LOCK"},{0x2C,"KEY_PRT_SCR"},{ 0x14,"KEY_CAPS_LOCK" },{ 0x70,"KEY_F1" },
		{ 0x71,"KEY_F2" },{ 0x72,"KEY_F3" },{ 0x73,"KEY_F4" },{ 0x74,"KEY_F5" },{ 0x75,"KEY_F6" },{ 0x76,"KEY_F7" },
		{ 0x77,"KEY_F8" },{ 0x78,"KEY_F9" },{ 0x79,"KEY_F10" },{ 0x7A,"KEY_F11" },{ 0x7B,"KEY_F12" },{ 0x20,"KEY_SPACE" },
		{ 0xA0,"KEY_LSHIFT" },{ 0xA2,"KEY_LCTR" },{0xA1,"KEY_RSHIFT"},{0xA3,"KEY_RCTR"},{ 0xA4,"KEY_LALT" },
		{0xA5,"KEY_RALT"},{ 0x5B,"KEY_LWIN" },{ 0x5C,"KEY_RWIN" },{0x5D,"KEY_APPS"},};
}

RecordMode::RecordMode(BlockingQueue<std::shared_ptr<IMessage>> &que, boost::program_options::variables_map const& vm):FreeRunningMode{ que,vm },bque{que},buffer{}
{
	fileName=vm["record"].as<std::string>();
	if(vm.count("keyboard"))
	{
		keyboard.connect(std::bind(&RecordMode::saveKeyboard,this,std::placeholders::_1,std::placeholders::_2));
	}
	if(vm.count("mouse"))
	{
		mouse.connect2(std::bind(&RecordMode::saveMouse, this, std::placeholders::_1,std::placeholders::_2));
	}

}

RecordMode::RecordMode(RecordMode const& r) :FreeRunningMode{ r },bque{ r.bque }
{
	fileName = r.fileName;
}

RecordMode::~RecordMode()
{
}

void RecordMode::Run()
{
	FreeRunningMode::Run();
	file.open(fileName, std::ofstream::out);
	file << SHORT_FLAG<<'\n';
	file<<buffer.str();
	file.close();
}

void RecordMode::saveMouse(MouseReport report, std::string name)
{
	if(name=="mouseRelease")
		name="mouseReleaseAll";
	buffer << name<<' ';
	if(name=="mousePush")
		buffer<<((report.buttons==1)?"LPM":"PPM");
	else if(name=="mouseMove")
		buffer<<std::to_string(report.X)<<','<<std::to_string(report.Y)<<','<<std::to_string(report.Wheel);
	else if(name=="mouseReleaseAll")
		buffer<<"0";
	buffer<<'\n';
}

void RecordMode::saveKeyboard(uint32_t key, std::string name)
{
	buffer << name << ' ';
	if (MP::npp.count(key))
		buffer<<MP::npp[key];
	else
		buffer<<scan2ascii(key);
	buffer<<'\n';
}

uint8_t RecordMode::scan2ascii(DWORD vk)
{
	static HKL layout = GetKeyboardLayout(0);
	static uint8_t State[256];
	uint16_t result=1;
	if (GetKeyboardState(State) == FALSE)
		return 0;
	UINT scan = MapVirtualKeyEx(vk,0,layout);
	auto re=ToAsciiEx(vk, scan, State, &result, 0,layout);
	return static_cast<uint8_t>(result);

}
