#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(bool active) : Module(Module::Type::Audio, active) {}

bool ModuleAudio::init() 
{
	return true;
}

bool ModuleAudio::cleanUp()
{
	return true;
}
