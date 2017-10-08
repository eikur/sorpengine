#include "ModuleInput.hpp"
#include "ModuleWindow.hpp"
#include "Utils.hpp"
#include "Application.hpp"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput(bool active) 
	: Module(Module::Type::Input, active)
{
	_keyboard = new KeyState[numKeys];
	_mouseButtons = new KeyState[numMouseButttons];
	memset(_keyboard, KeyState::Idle, sizeof(KeyState)*numKeys);
	memset(_mouseButtons, KeyState::Idle, sizeof(KeyState)*numMouseButttons);
}

ModuleInput::~ModuleInput()
{
	delete[] _keyboard;
	delete[] _mouseButtons;
}

bool ModuleInput::init() 
{
	Utils::log("Init SDL input event system");

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		Utils::log("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

UpdateStatus ModuleInput::preUpdate()
{
	UpdateStatus retVal = UpdateStatus::Continue;
	static SDL_Event sdlEvent;
	_mouseMotion = { 0, 0 };
	_mouseWheelRoll = 0;

	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	for (int i = 0; i < numKeys; ++i)
	{
		if (keys[i] == 1)
		{
			_keyboard[i] = _keyboard[i] == KeyState::Idle ? KeyState::Down : KeyState::Repeat;
		}
		else
		{
			_keyboard[i] = _keyboard[i] == KeyState::Down ? KeyState::Up : (_keyboard[i] == KeyState::Repeat ? KeyState::Up : KeyState::Idle);
		}
	}

	for (int i = 0; i < numMouseButttons; ++i)
	{
		if (_mouseButtons[i] == KeyState::Down)
		{
			_mouseButtons[i] = KeyState::Repeat;
		}
		if (_mouseButtons[i] == KeyState::Up)
		{
			_mouseButtons[i] = KeyState::Idle;
		}
	}

	while (SDL_PollEvent(&sdlEvent) != 0)
	{
		switch (sdlEvent.type)
		{
			case SDL_QUIT:
				retVal = UpdateStatus::Stop;
			break;

			case SDL_WINDOWEVENT:
				switch (sdlEvent.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					App->getModule<ModuleWindow>().setWindowSize({ static_cast<int>(sdlEvent.window.data1), static_cast<int>(sdlEvent.window.data2) });
					break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				_mouseButtons[sdlEvent.button.button - 1] = KeyState::Down;
				break;

			case SDL_MOUSEBUTTONUP:
				_mouseButtons[sdlEvent.button.button - 1] = KeyState::Up;
				break;

			case SDL_MOUSEMOTION:
				_mouseMotion.x = sdlEvent.motion.xrel;
				_mouseMotion.y = sdlEvent.motion.yrel;
				_mousePosition.x = sdlEvent.motion.x;
				_mousePosition.y = sdlEvent.motion.y;
				break;

			case SDL_MOUSEWHEEL:
				_mouseWheelRoll = sdlEvent.wheel.y;
				break;
		}
	}

	return retVal;
}

bool ModuleInput::cleanUp()
{
	Utils::log("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

ModuleInput::KeyState ModuleInput::getKey(int id) const
{
	return _keyboard[id];
}

ModuleInput::KeyState ModuleInput::getMouseButtonDown(int id) const
{
	return _mouseButtons[id];
}

const iPoint& ModuleInput::getMouseMotion() const
{
	return _mouseMotion;
}

const iPoint& ModuleInput::getMousePosition() const
{
	return _mousePosition;
}

const Sint32 ModuleInput::getMouseWheelRoll() const
{
	return _mouseWheelRoll;
}