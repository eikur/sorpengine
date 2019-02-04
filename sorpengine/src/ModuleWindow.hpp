#pragma once

#include "Module.hpp"
#include "Point.hpp"
#include "SceneManager.hpp"
#include "SDL.h"
#include <memory>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;

class ModuleWindow : public Module
{
	public:
		using iPoint = Point<int>;

		ModuleWindow(SceneManager& sceneManager, bool active = true);
		virtual ~ModuleWindow() {}

		bool init() override;
		UpdateStatus preUpdate() override;
		UpdateStatus postUpdate() override;
		bool cleanUp() override;

		const iPoint& getWindowSize() const;
		SDL_Window* getSDLWindow() const;

	private:
		void updateCameraWindow();

		SDL_Window* _sdlWindow = nullptr;
		SDL_Surface* _sdlSurface = nullptr;
		SDL_Renderer* _sdlRenderer = nullptr;
		SDL_GLContext _glContext;

        SceneManager& _sceneManager;

		bool _fullScreen = false;
		bool _resizable = false; // disabled for now
		bool _vsync = true;

        iPoint _windowSize = { 1800, 900 };
        iPoint _sceneViewMargin = { 280, 0 };
        iPoint _sceneViewSize;
};
