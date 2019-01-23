#pragma once

#include "Camera.hpp"
#include "Module.hpp"
#include "Point.hpp"
#include "SDL.h"
#include <memory>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;

class ModuleWindow : public Module
{
	public:
		using iPoint = Point<int>;

		ModuleWindow(bool active = true);
		virtual ~ModuleWindow() {}

		bool init() override;
		UpdateStatus preUpdate() override;
		UpdateStatus update(float dt = 0.0f) override;
		UpdateStatus postUpdate() override;
		bool cleanUp() override;

		void setWindowSize(const iPoint& newSize);
		const iPoint& getWindowSize() const;
		SDL_Window* getSDLWindow() const;
		
		void toggleCameraMode();

	private:
		void updateWindow();

		SDL_Window* _sdlWindow = nullptr;
		SDL_Surface* _sdlSurface = nullptr;
		SDL_Renderer* _sdlRenderer = nullptr;
		SDL_GLContext _glContext;

		Camera _camera;


		bool _fullScreen = false;
		bool _resizable = true;
		bool _vsync = true;
		bool _isDirty = false;

		iPoint _windowSize = { 600, 480 };
};
