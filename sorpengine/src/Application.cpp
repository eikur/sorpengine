/* Application.cpp */

#include "Application.h"
#include "Module.h"
#include "ModuleWindow.h"
#include <algorithm>

namespace 
{
	const std::vector<Module*> getActiveModules(const std::vector<std::unique_ptr<Module>>& allModules)
	{
		std::vector<Module*> activeModules;

		for (auto&& module : allModules)
		{
			if (module->isActive())
			{
				activeModules.push_back(&*module);
			}
		}
		return std::move(activeModules);
	}
}

Application::Application()
{
	auto moduleWindow = std::make_unique<ModuleWindow>();
	modules.push_back(std::move(moduleWindow));
}

Application::~Application()
{
	for (auto& module : modules)
	{
		delete module.get();
		module.release();
	}
}

bool Application::Init()
{
	bool ret = true;

	for (auto&& module : modules)
	{
		ret = ret && module->init();
	}
	
	for (auto&& module : getActiveModules(modules))
	{
		ret = ret && module->start();
	}

	return ret;
}

UpdateStatus Application::Update()
{
	UpdateStatus ret = UpdateStatus::Continue;

	const std::vector<Module*>& activeModules = getActiveModules(modules);

	for (auto&& module : activeModules)
	{
		ret = (ret == UpdateStatus::Continue) ? module->preUpdate() : ret;
	}

	for (auto&& module : activeModules)
	{
		ret = (ret == UpdateStatus::Continue) ? module->update() : ret;
	}

	for (auto&& module : activeModules)
	{
		ret = (ret == UpdateStatus::Continue )? module->postUpdate() : ret;
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	const std::vector<Module*>& activeModules = getActiveModules(modules);

	for (auto& it = activeModules.rbegin(); it != activeModules.rend() && ret; ++it)
		ret = (*it)->cleanUp();

	return ret;
}