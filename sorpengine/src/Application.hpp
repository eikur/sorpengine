/* Application.h */

#pragma once
#include <memory>
#include <vector>
#include "Globals.hpp"
#include "Module.hpp"
#include "Macros.hpp"

class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleGUI;
class SceneManager;
class ShaderManager;
class TextureHelper;
class ModelHelper;
class TimeManager;

class Application
{
    enum class Mode
    {
        Edit,
        Game
    };

public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

    void enterGameMode();
    void togglePauseGameMode();
    void exitGameMode();
    
    bool isInGameMode() const;
    bool isInEditMode() const;

    bool isGameModePaused() const;

    float getFrameRate() const;

private:
    Mode _mode = Mode::Edit;

	std::vector<Module*> _modules;
	MEMBER_DECL(ModuleWindow, _window, getWindow)
	MEMBER_DECL(ModuleInput, _input, getInput)
	MEMBER_DECL(ModuleAudio, _audio, getAudio)
	MEMBER_DECL(SceneManager, _sceneManager, getSceneManager)
	MEMBER_DECL(ModuleGUI, _gui, getGUI)

	MEMBER_DECL(ShaderManager, _shaderManager, getShaderManager)
	MEMBER_DECL(TextureHelper, _textureHelper, getTextureHelper)
    MEMBER_DECL(ModelHelper, _modelHelper, getModelHelper)
    MEMBER_DECL(TimeManager, _timeManager, getTimeManager)
};

extern Application* App;