#pragma once
#include <stdint.h>
#include <array>
typedef uint32_t keyType;
class IKeyboard
{
public:
	IKeyboard();
	virtual void push(keyType key)=0;
	virtual void release(keyType key)=0;
	virtual void releaseAll()=0;
	virtual ~IKeyboard();
};

