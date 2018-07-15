#pragma once
#include <algorithm>
#include "Globals.hpp"

class GameObject;

class Component
{
	enum class Type
	{
		Transform,
		Mesh,
		Material
	};

public:
	Component(Type type, bool active = true);
	void setActive(bool value);

	virtual bool init();
	virtual bool start();
	virtual UpdateStatus preUpdate();
	virtual UpdateStatus update(float dt = 0.0f);
	virtual UpdateStatus postUpdate();
	virtual bool cleanUp();

private:
	bool _active = true;
};