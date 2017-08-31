#pragma once
#include "IKeyboard.h"


class ITranslator
{
public:
	ITranslator();
	virtual keyType Translate(keyType key)=0;
	virtual ~ITranslator() = default;
};

