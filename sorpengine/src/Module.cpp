#include "Module.h"

bool Module::SetActive(bool active)
{
	if (_active != active)
	{
		_active = active ? Start() : !CleanUp();
	}
	return _active;
}

bool Module::IsActive() const
{
	return _active;
}

bool Module::Init() 
{ 
	return true; 
}

bool Module::Start()
{
	return true;
}

UpdateStatus Module::PreUpdate()
{
	return UpdateStatus::Continue;
}

UpdateStatus Module::Update()
{
	return UpdateStatus::Continue;
}

UpdateStatus Module::PostUpdate()
{
	return UpdateStatus::Continue;
}

bool Module::CleanUp()
{
	return true;
}

const std::string& Module::getId()
{
	return _id;
}

