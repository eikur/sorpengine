#include "ModuleWindow.hpp"

#include "Globals.hpp"
#include "Utils.hpp"
#include "GL/glew.h"
// remove me
#include "ModuleInput.hpp"
#include "Application.hpp"

ModuleWindow::ModuleWindow(bool active): Module(active) 
{
}

bool ModuleWindow::init()
{
	Utils::log("Init SDL window & surface");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Utils::log("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	
	Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	flags = _fullScreen ? flags | SDL_WINDOW_FULLSCREEN : flags;
	flags = _resizable ? flags | SDL_WINDOW_RESIZABLE : flags;

	_sdlWindow = SDL_CreateWindow("Engine Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowSize.x, _windowSize.y, flags);
	if (_sdlWindow == nullptr)
	{
		Utils::log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	_sdlSurface = SDL_GetWindowSurface(_sdlWindow);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	Utils::log("Creating 3D renderer context");
	_glContext = SDL_GL_CreateContext(_sdlWindow);
	if (_glContext == nullptr)
	{
		Utils::log("OpenGL context could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	GLenum errorCode = glewInit();
	if (errorCode != GLEW_OK)
	{
		Utils::log("Glew library could not init. Error: %s\n", glewGetErrorString(errorCode));
		return false;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	errorCode = glGetError();
	if (errorCode != GL_NO_ERROR)
	{
		Utils::log("Error initializing OpenGL! %s\n", gluErrorString(errorCode));
		return false;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	errorCode = glGetError();
	if (errorCode != GL_NO_ERROR)
	{
		Utils::log("Error initializing OpenGL! %s\n", gluErrorString(errorCode));
		return false;
	}

	flags = _vsync ? SDL_RENDERER_PRESENTVSYNC : 0;
	SDL_GL_SetSwapInterval(_vsync ? 1 : 0);
	_sdlRenderer = SDL_CreateRenderer(_sdlWindow, -1, flags);
	if (_sdlRenderer == nullptr)
	{
		Utils::log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHT0);
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    _sceneViewSize = { _windowSize.x - 2 * _sceneViewMargin.x, _windowSize.y - _sceneViewMargin.y };
    _editorCamera.Init(_sceneViewSize.x / (float)_sceneViewSize.y);
    _currentCamera = &_editorCamera;
	_isDirty = true;

    return true;
}


UpdateStatus ModuleWindow::preUpdate()
{
    if (_isDirty)
    {
        updateCameraWindow();
        _isDirty = false;
    }
    
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(_sceneViewMargin.x, _sceneViewMargin.y, _sceneViewSize.x, _sceneViewSize.y);

    glEnable(GL_SCISSOR_TEST);
    glScissor(_sceneViewMargin.x, _sceneViewMargin.y, _sceneViewSize.x, _sceneViewSize.y);
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    // glClearColor(_currentCamera->getClearColor()); // move clear color to camera
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

	return UpdateStatus::Continue;
}

UpdateStatus ModuleWindow::update(float /*dt*/)
{
    // These are camera controls. Consider moving them when the camera is converted to a component
    // Editor camera controls

	if (App->getInput().getKey(SDL_SCANCODE_C) == ModuleInput::KeyState::Down)
	{
        toggleCameraMode();
	}

    const float kCamMoveSpeed = 3.0f;
    const float dt = 0.02f;

    if (App->getInput().getKey(SDL_SCANCODE_W) == ModuleInput::KeyState::Repeat)
    {
        translateCamera(_editorCamera.front() * kCamMoveSpeed * dt );
    }
    else if (App->getInput().getKey(SDL_SCANCODE_S) == ModuleInput::KeyState::Repeat)
    {
        translateCamera(-_editorCamera.front() * kCamMoveSpeed * dt);
    }

    if (App->getInput().getKey(SDL_SCANCODE_Q) == ModuleInput::KeyState::Repeat)
    {
        translateCamera(-_editorCamera.up() * kCamMoveSpeed * dt);
    }
    else if (App->getInput().getKey(SDL_SCANCODE_E) == ModuleInput::KeyState::Repeat)
    {
        translateCamera(_editorCamera.up() * kCamMoveSpeed * dt);
    }

    if (App->getInput().getKey(SDL_SCANCODE_A) == ModuleInput::KeyState::Repeat)
    {
        translateCamera(-_editorCamera.right() * kCamMoveSpeed * dt);
    }
    else if (App->getInput().getKey(SDL_SCANCODE_D) == ModuleInput::KeyState::Repeat)
    {
        translateCamera(_editorCamera.right() * kCamMoveSpeed * dt);
    }
    
	return UpdateStatus::Continue;
}

UpdateStatus ModuleWindow::postUpdate()
{
	SDL_GL_SwapWindow(_sdlWindow);
	return UpdateStatus::Continue;
}

bool ModuleWindow::cleanUp()
{
	Utils::log("Destroying SDL window and quitting all SDL systems");
	SDL_GL_DeleteContext(_glContext);
	if (_sdlRenderer != nullptr)
	{
		SDL_DestroyRenderer(_sdlRenderer);
	}

	if (_sdlWindow != nullptr)
	{
		SDL_DestroyWindow(_sdlWindow);
	}
	SDL_Quit();

	return true;
}

void ModuleWindow::setWindowSize(const iPoint& newSize)
{
	if (_windowSize == newSize)
	{
		return;
	}
	_windowSize = newSize;
    _editorCamera.setAspectRatio(_windowSize.x / (float)_windowSize.y);
	_isDirty = true;
}

const iPoint& ModuleWindow::getWindowSize() const
{
	return _windowSize;
}

void ModuleWindow::updateCameraWindow()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(_currentCamera->getProjectionMatrix().ptr());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(_currentCamera->getViewMatrix().ptr());
}

SDL_Window* ModuleWindow::getSDLWindow() const
{
	return _sdlWindow;
}

void ModuleWindow::toggleCameraMode()
{
	_editorCamera.switchType();
	_isDirty = true;
}

void ModuleWindow::translateCamera(const float3& translation)
{
    _editorCamera.translate(translation);
    _isDirty = true;
}

void ModuleWindow::showCameraProperties()
{
    _editorCamera.onEditor();
    _isDirty = true;
}