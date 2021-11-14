#include "Module.hpp"

bool Module::init() 
{ 
	return true; 
}

bool Module::start()
{
	return true;
}

UpdateStatus Module::preUpdate()
{
	return UpdateStatus::Continue;
}

UpdateStatus Module::update(float)
{
	return UpdateStatus::Continue;
}

UpdateStatus Module::postUpdate()
{
	return UpdateStatus::Continue;
}

bool Module::cleanUp()
{
	return true;
}


