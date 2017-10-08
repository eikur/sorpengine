#pragma once
#include "Module.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio(bool active = true);
	virtual ~ModuleAudio() {}

	bool init() override;
	bool cleanUp() override;

};
