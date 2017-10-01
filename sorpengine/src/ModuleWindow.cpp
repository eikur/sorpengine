#include "ModuleWindow.h"
#include "Globals.h"
#include "Utils.h"

#include "SDL/include/SDL.h"

ModuleWindow::ModuleWindow(bool active): Module("window", active) {}

bool ModuleWindow::init()
{
	Utils::log("Init SDL window & surface");
	bool ret = true;

	Utils::log("%s init\n", getId().c_str());
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Utils::log("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret)
	{
		Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;

		if (_fullScreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (_resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		_sdlWindow = SDL_CreateWindow("sorpengine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 480, flags);

		if (_sdlWindow == nullptr)
		{
			Utils::log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			_sdlSurface = SDL_GetWindowSurface(_sdlWindow);
		}
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	return ret;
}

UpdateStatus ModuleWindow::update(float)
{
	return UpdateStatus::Continue;
}

bool ModuleWindow::cleanUp()
{
	Utils::log("Destroying SDL window and quitting all SDL systems");

	if (_sdlWindow != nullptr)
	{
		SDL_DestroyWindow(_sdlWindow);
	}
	SDL_Quit();

	return true;
}