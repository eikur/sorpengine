#include "ModuleWindow.h"
#include "Globals.h"
#include "Utils.h"
#include "glew-2.0.0/include/GL/glew.h"

ModuleWindow::ModuleWindow(bool active): Module(Module::Type::Window, active) {}

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

	_sdlWindow = SDL_CreateWindow("sorpengine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowSize.x, _windowSize.y, flags);
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

	return true;
}


UpdateStatus ModuleWindow::preUpdate()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UpdateStatus::Continue;
}

UpdateStatus ModuleWindow::update(float)
{
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
	_windowSize = newSize;
}

const iPoint& ModuleWindow::getWindowSize() const
{
	return _windowSize;
}