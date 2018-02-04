#include "Module.hpp"

bool Module::setActive(bool active)
{
	if (_active != active)
	{
		_active = active ? start() : !cleanUp();
	}
	return _active;
}

bool Module::isActive() const
{
	return _active;
}

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


