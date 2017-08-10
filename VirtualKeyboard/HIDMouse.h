#pragma once
#include "IMouse.h"
class HIDMouse:public IMouse
{
public:
	HIDMouse();
	~HIDMouse();

	// Odziedziczono za poœrednictwem elementu IMouse
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll() override;
	virtual void updatePosition(int x, int y) override;
	virtual void updateWheel(int distance) override;
private:
	MouseReport mouseReport;
};

