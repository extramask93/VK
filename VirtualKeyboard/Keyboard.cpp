#include "stdafx.h"
#include "Keyboard.h"
#include <boost/asio/detail/buffer_sequence_adapter.hpp>


boost::signals2::connection Keyboard::connect(const signal2_t::slot_type& subscriber)
{
	return keyChanged.connect(subscriber);
}

void Keyboard::push(keyType key)
{
	for (auto &k : keys)
	{
		if (k == key)
			return;
	}
	for (auto &k : keys)
	{
		if (k == 0x00)
		{
			k = key;
			keyChanged(key, "keyboardPush");
			auto temp=translator.Translate(k);
			keyboard.push(temp);
			break;
		}
	}
}
void Keyboard::release(keyType key)
{
	for (auto &k : keys)
	{
		if (k == key) {
			k = 0x00;
			keyChanged(key, "keyboardRelease");
			keyboard.release(translator.Translate(key));
		}
	}
}
void Keyboard::releaseAll()
{
	for (auto &k : keys)
		k = 0x00;
	keyboard.releaseAll();
}

std::array<keyType, MAX_KEYS> Keyboard::getKeys() const
{
	return keys;
}


Keyboard::~Keyboard()
{
}

