#pragma once
#include "Module.hpp"
#include "SDL/include/SDL_scancode.h"
#include "Point.hpp"
#include <map>

class ModuleInput : public Module
{
public:
	using iPoint = Point<int>;

	enum KeyState
	{
		Idle = 0,
		Down,
		Repeat,
		Up
	};

private:
	const int numMouseButttons = 5;
	const int numKeys = 300;

public:
	ModuleInput(bool active = true);
	virtual ~ModuleInput();

	bool init() override;
	UpdateStatus preUpdate() override;
	bool cleanUp() override;

	KeyState getKey(int id) const;
	KeyState getMouseButtonDown(int id) const;

	// Get mouse / axis position
	const iPoint& getMouseMotion() const;
	const iPoint& getMousePosition() const;

	// Get mouse wheel roll
	const Sint32 getMouseWheelRoll() const;

private:
	KeyState* _keyboard = nullptr;
	KeyState* _mouseButtons = nullptr;
	iPoint _mouseMotion = { 0,0 };
	iPoint _mousePosition = { 0,0 };
	Sint32 _mouseWheelRoll = 0;
};
