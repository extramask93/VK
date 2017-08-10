#pragma once
#include "IKeyboard.h"


class ITranslator
{
public:
	ITranslator();
	virtual keyType translate(keyType key)=0;
};

