#pragma once
#include "IRunMode.h"
class MacroMode :
	public IRunMode
{
public:
	MacroMode();
	~MacroMode();

	// Odziedziczono za po�rednictwem elementu IRunMode
	virtual void Run() override;
};

