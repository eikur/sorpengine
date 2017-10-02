#pragma once
#include "Module.h"
#include "SDL/include/SDL_scancode.h"
#include "Point.h"
#include <map>

class ModuleInput : public Module
{
public:
	using iPoint = Point<int>;

private:
	enum KeyState
	{
		Idle = 0,
		Down,
		Repeat,
		Up
	};
	const int numMouseButttons = 5;
	const int numKeys = 300;

public:
	ModuleInput(bool active = true);
	virtual ~ModuleInput();

	bool init() override;
	UpdateStatus preUpdate() override;
	bool cleanUp() override;

	KeyState GetKey(int id) const;
	KeyState GetMouseButtonDown(int id) const;

	// Get mouse / axis position
	const iPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;

	// Get mouse wheel roll
	const int GetMouseWheelRoll() const;

private:
	KeyState* _keyboard = nullptr;
	KeyState* _mouseButtons = nullptr;
	iPoint _mouseMotion = { 0,0 };
	iPoint _mousePosition = { 0,0 };
	int _mouseWheelRoll = 0;
};
