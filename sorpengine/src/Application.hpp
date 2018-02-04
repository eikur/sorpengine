/* Application.h */

#pragma once
#include <memory>
#include <vector>
#include "Globals.hpp"
#include "Module.hpp"
#include "Utils/Macros.hpp"

class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class SceneManager;
class ShaderManager;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

private:
	std::vector<Module*> _modules;
	MEMBER_DECL(ModuleWindow, _window, getWindow)
	MEMBER_DECL(ModuleInput, _input, getInput)
	MEMBER_DECL(ModuleAudio, _audio, getAudio)
	MEMBER_DECL(SceneManager, _sceneManager, getSceneManager)

	MEMBER_DECL(ShaderManager, _shaderManager, getShaderManager)

};

extern Application* App;