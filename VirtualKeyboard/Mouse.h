#pragma once
#include "Keyboard.h"
#include "IMouse.h"
#include <boost\signals2.hpp>
class Mouse:public IMouse
{
public:
	typedef boost::signals2::signal<void(MouseReport)> signal_t;
	Mouse(IMouse &mouse) :mouse{ mouse } {}
	boost::signals2::connection connect(signal_t::slot_type & subscribent);
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll() override;
	virtual void updatePosition(int x, int y) override;
	virtual void updateWheel(int distance) override;
private:
	MouseState state;
	signal_t reportChanged;
	IMouse &mouse;
	// Odziedziczono za poœrednictwem elementu IMouse

};

