/* Application.cpp */

#include "Application.hpp"
#include "ModuleWindow.hpp"
#include "ModuleInput.hpp"
#include "ModuleAudio.hpp"
#include "ModuleScene.hpp"
#include "ShaderManager.hpp"
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
	_modules.push_back(std::make_unique<ModuleWindow>());
	_modules.push_back(std::make_unique<ModuleInput>());
	_modules.push_back(std::make_unique<ModuleAudio>());
	_modules.push_back(std::make_unique<ModuleScene>());

	_shaderManager = std::make_unique<ShaderManager>();
}

Application::~Application()
{
	for (auto& module : _modules)
	{
		delete module.get();
		module.release();
	}

	delete _shaderManager.get();
	_shaderManager.release();
}

bool Application::Init()
{
	bool ret = true;

	for (auto&& module : _modules)
	{
		ret = ret && module->init();
	}
	
	for (auto&& module : getActiveModules(_modules))
	{
		ret = ret && module->start();
	}

	_shaderManager->init();

	return ret;
}

UpdateStatus Application::Update()
{
	UpdateStatus ret = UpdateStatus::Continue;

	const std::vector<Module*>& activeModules = getActiveModules(_modules);

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

	const std::vector<Module*>& activeModules = getActiveModules(_modules);

	for (auto& it = activeModules.rbegin(); it != activeModules.rend() && ret; ++it)
		ret = (*it)->cleanUp();

	_shaderManager.get()->CleanUp();

	return ret;
}

//-----------------------
Module* Application::findModule(Module::Type type) const
{
	auto it = std::find_if(_modules.begin(), _modules.end(), [type](const std::unique_ptr<Module>& uniquePtr) {return uniquePtr.get()->getType() == type; });
	return (*it).get();
}

template <>
ModuleWindow& Application::getModule() const
{
	return *static_cast<ModuleWindow*>(findModule(Module::Type::Window));
}

template <>
ModuleInput& Application::getModule() const
{
	return *static_cast<ModuleInput*>(findModule(Module::Type::Input));
}

template <>
ModuleAudio& Application::getModule() const
{
	return *static_cast<ModuleAudio*>(findModule(Module::Type::Audio));
}

template <>
ModuleScene& Application::getModule() const
{
	return *static_cast<ModuleScene*>(findModule(Module::Type::Scene));
}

ShaderManager& Application::getShaderManager() const
{
	return *_shaderManager.get();
}