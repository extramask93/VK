#pragma once
#include "IRunMode.h"
class MacroMode :
	public IRunMode
{
public:
	MacroMode();
	~MacroMode();

	// Odziedziczono za poœrednictwem elementu IRunMode
	virtual void Run() override;
};

