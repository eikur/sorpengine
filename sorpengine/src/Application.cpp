/* Application.cpp */

#include "Application.h"
#include "Module.h"
#include <algorithm>

namespace 
{
	const std::vector<Module*> getActiveModules(const std::vector<std::unique_ptr<Module>>& allModules)
	{
		std::vector<Module*> activeModules;

		for (auto&& module : allModules)
		{
			if (module->IsActive())
			{
				activeModules.push_back(&*module);
			}
		}
		return std::move(activeModules);
	}
}

Application::Application()
{
	auto dummyModule = std::make_unique<Module>("dummy", true);
	modules.push_back(std::move(dummyModule));
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
		ret = ret && module->Init();
	}
	
	for (auto&& module : modules)
	{
		if (module->IsActive())
		{
			ret = ret && module->Start();
		}
	}

	return ret;
}

UpdateStatus Application::Update()
{
	UpdateStatus ret = UpdateStatus::Continue;

	const std::vector<Module*>& activeModules = getActiveModules(modules);

	for (auto&& module : activeModules)
	{
		ret = (ret == UpdateStatus::Continue) ? module->PreUpdate() : ret;
	}

	for (auto&& module : activeModules)
	{
		ret = (ret == UpdateStatus::Continue) ? module->Update() : ret;
	}

	for (auto&& module : activeModules)
	{
		ret = (ret == UpdateStatus::Continue )? module->PostUpdate() : ret;
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	const std::vector<Module*>& activeModules = getActiveModules(modules);

	for (auto& it = activeModules.rbegin(); it != activeModules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}