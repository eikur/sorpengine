#include "ModuleInput.h"
#include "Utils.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput(bool active) 
	: Module("input", active)
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