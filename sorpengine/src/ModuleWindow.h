#pragma once

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;

class ModuleWindow : public Module
{
	public:
		ModuleWindow(bool active = true);
		virtual ~ModuleWindow() {}

		bool init() override;
		UpdateStatus preUpdate() override;
		UpdateStatus update(float dt = 0.0f) override;
		UpdateStatus postUpdate() override;
		bool cleanUp() override;
	
	private:
		SDL_Window* _sdlWindow = nullptr;
		SDL_Surface* _sdlSurface = nullptr;
		SDL_Renderer* _sdlRenderer = nullptr;
		SDL_GLContext _glContext;

		bool _fullScreen = false;
		bool _resizable = true;
		bool _vsync = true;
};
