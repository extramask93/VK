#include "stdafx.h"
#include "HIDKeyboard.h"


HIDKeyboard::HIDKeyboard()
{
	report.id = 1;
}
bool HIDKeyboard::isModifier(keyType key) const
{
	if ((key >= 0xE0 && key <= 0xE7))
		return true;
	else
		return false;
}
void HIDKeyboard::push(keyType key)
{
	if (isModifier(key))
	{
		report.modifiers |= (1 << (key - 128));
		reportChanged(report);
		return;
	}
		for (auto &k : report.keys)
		{
			if (key == k)
				return;
		}
	//add to already pressed
	for (auto &k : report.keys)
	{
		if (k == 0x00)
		{
			k = key;
			reportChanged(report);
			break;
		}
	}

}

void HIDKeyboard::release(keyType key)
{
	if (isModifier(key))
	{
		report.modifiers &= ~(1 << (key - 128));
		reportChanged(report);
		return;
	}
	for (auto &k : report.keys)
	{
		if (key == k)
		{
			k = 0x00;
			reportChanged(report);
			break;
		}
	}
}

void HIDKeyboard::releaseAll()
{
	report.modifiers = 0;
	for(auto &k:report.keys)
	{
		k = 0;
	}
	reportChanged(report);
}
boost::signals2::connection HIDKeyboard::connect(const signal_t::slot_type & subscriber)
{
	return reportChanged.connect(subscriber);
}

void HIDKeyboard::disconnectAll()
{
	reportChanged.disconnect_all_slots();
}


Report::Report()
{
	id = 1;
	reserved = modifiers = 0;
	for (auto& key : keys)
	{
		key = 0;
	}
}

uint8_t Report::getSize()
{
	return (sizeof(id) + sizeof(modifiers) + sizeof(reserved) + keys.size());
}

std::vector<uint8_t> Report::getFields()
{
	std::vector<uint8_t> vu;
	vu.push_back(0x30);
	vu.push_back(id);
	vu.push_back(modifiers);
	vu.push_back(keys[0]);
	vu.push_back(keys[1]);
	vu.push_back(keys[2]);
	vu.push_back(keys[3]);
	vu.push_back(keys[4]);
	vu.push_back(keys[5]);
	return vu;
}

uint8_t Report::getPacketId()
{
	return KEYBOARD_WRITE_REQ;
}
