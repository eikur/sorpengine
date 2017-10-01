#pragma once

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
	public:
		ModuleWindow(bool active = true);
		virtual ~ModuleWindow() {}

		bool init() override;
		UpdateStatus update(float dt = 0.0f) override;
		bool cleanUp() override;
	
	private:
		SDL_Window* _sdlWindow = nullptr;
		SDL_Surface* _sdlSurface = nullptr;
		bool _fullScreen = false;
		bool _resizable = true;
};
