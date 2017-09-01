#pragma once
#include "IKeyboard.h"

//!Interface for translators from one keyboard standard to another(vk_code->HID)
class ITranslator
{
public:
	ITranslator();
	virtual keyType Translate(keyType key)=0;
	virtual ~ITranslator() = default;
};

