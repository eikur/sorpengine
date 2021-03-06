/* Application.cpp */

#include "Application.hpp"
#include "ModelHelper.hpp"
#include "ModuleWindow.hpp"
#include "ModuleInput.hpp"
#include "ModuleAudio.hpp"
#include "ModuleGUI.hpp"
#include "TextureHelper.hpp"
#include "TimeManager.hpp"
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
    _timeManager = std::make_unique<TimeManager>();

	_input = std::make_unique<ModuleInput>();
	_sceneManager = std::make_unique<SceneManager>(*this, *_input);
	_window = std::make_unique<ModuleWindow>(*_sceneManager);
    _audio = std::make_unique<ModuleAudio>();
	_gui = std::make_unique<ModuleGUI>(*this, *_sceneManager, *_window);

	_modules = { _window.get(), _input.get(), _audio.get(), _sceneManager.get(), _gui.get()};
	_shaderManager = std::make_unique<ShaderManager>();
	_textureHelper = std::make_unique<TextureHelper>();
    _modelHelper = std::make_unique<ModelHelper>(*_textureHelper.get());
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

    _timeManager.reset();
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
		if (module->isActive())
		{
			ret = ret && module->start();
		}
	}

	_shaderManager->init();
	_textureHelper->init();
    _modelHelper->init();

	return ret;
}

UpdateStatus Application::Update()
{
    _timeManager->frameWasCompleted();

	UpdateStatus ret = UpdateStatus::Continue;

	const std::vector<Module*>& activeModules = getActiveModules(_modules);

	for (auto&& module : activeModules)
	{
		ret = (ret == UpdateStatus::Continue) ? module->preUpdate() : ret;
	}

	for (auto&& module : activeModules)
	{
		ret = (ret == UpdateStatus::Continue) ? module->update(_timeManager->getGameDelta()) : ret;
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


void Application::enterGameMode()
{
    _mode = Mode::Game;
    _timeManager->startGame();
}
void Application::exitGameMode()
{
    _mode = Mode::Edit;
    _timeManager->stopGame();
}

void Application::togglePauseGameMode()
{
    if (_mode == Mode::Edit)
    {
        return;
    }
    _timeManager->togglePauseGame();
}

bool Application::isInEditMode() const
{
    return _mode == Mode::Edit;
}

bool Application::isInGameMode() const
{
    return _mode == Mode::Game;
}

bool Application::isGameModePaused() const
{
    return isInGameMode() && _timeManager->isGamePaused();
}

float Application::getFrameRate() const
{
    return _timeManager->getAppFrameRate();
}