/* Application.cpp */

#include "Application.hpp"
#include "ModelHelper.hpp"
#include "ModuleWindow.hpp"
#include "ModuleInput.hpp"
#include "ModuleAudio.hpp"
#include "ModuleGUI.hpp"
#include "TextureHelper.hpp"
#include "SceneManager.hpp"
#include "ShaderManager.hpp"
#include <algorithm>

namespace 
{
	const std::vector<Module*> getActiveModules(const std::vector<Module*>& allModules)
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
	_window = std::make_unique<ModuleWindow>();
	_input = std::make_unique<ModuleInput>();
	_audio = std::make_unique<ModuleAudio>();
	_sceneManager = std::make_unique<SceneManager>();
	_gui = std::make_unique<ModuleGUI>(*_sceneManager);

	_modules = { _window.get(), _input.get(), _audio.get(), _sceneManager.get(), _gui.get()};
	_shaderManager = std::make_unique<ShaderManager>();
	_textureHelper = std::make_unique<TextureHelper>();
    _modelHelper = std::make_unique<ModelHelper>();
}

Application::~Application()
{
	_window.reset();
	_input.reset();
	_audio.reset();
	_gui.reset();
	_sceneManager.reset();

	_shaderManager.reset();
	_textureHelper.reset();
    _modelHelper.reset();
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
	_textureHelper->init();
    _modelHelper->init();

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

	for (auto it = activeModules.rbegin(); it != activeModules.rend() && ret; ++it)
	{
		ret = (*it)->cleanUp();
	}

	_shaderManager->finalize();
	_textureHelper->finalize();
    _modelHelper->finalize();

	return ret;
}
