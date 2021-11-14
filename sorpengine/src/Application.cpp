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

Application::Application()
{
	std::unique_ptr<ModuleInput> moduleInput = std::make_unique<ModuleInput>();
	std::unique_ptr<SceneManager> sceneManager = std::make_unique<SceneManager>(*this, *moduleInput);
	std::unique_ptr<ModuleWindow> moduleWindow = std::make_unique<ModuleWindow>(*sceneManager);
	std::unique_ptr<ModuleAudio> moduleAudio = std::make_unique<ModuleAudio>();
	std::unique_ptr<ModuleGUI> moduleGUI = std::make_unique<ModuleGUI>(*this, *sceneManager, *moduleWindow);

	_modules.push_back(std::move(moduleWindow));
	_modules.push_back(std::move(moduleInput));
	_modules.push_back(std::move(moduleAudio));
	_modules.push_back(std::move(sceneManager));
	_modules.push_back(std::move(moduleGUI));

	_timeManager = std::make_unique<TimeManager>();
	_shaderManager = std::make_unique<ShaderManager>();
	_textureHelper = std::make_unique<TextureHelper>();
    _modelHelper = std::make_unique<ModelHelper>(*_textureHelper.get());
}

Application::~Application() {}

bool Application::Init()
{
	bool ret = true;

	for (auto& module : _modules)
	{
		ret = ret && module->init();
	}
	
	for (auto& module : _modules)
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
    _timeManager->frameWasCompleted();

	UpdateStatus ret = UpdateStatus::Continue;

	for (auto& module : _modules)
	{
		ret = (ret == UpdateStatus::Continue) ? module->preUpdate() : ret;
	}

	for (auto& module : _modules)
	{
		ret = (ret == UpdateStatus::Continue) ? module->update(_timeManager->getGameDelta()) : ret;
	}

	for (auto& module : _modules)
	{
		ret = (ret == UpdateStatus::Continue )? module->postUpdate() : ret;
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (auto it = _modules.rbegin(); it != _modules.rend() && ret; ++it)
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