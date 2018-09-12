#pragma once
#include <algorithm>
#include "Globals.hpp"

class GameObject;

class Component
{
protected:
	enum class Type
	{
		Transform,
		Mesh,
		Material,
		Script
	};

public:
	Component(GameObject& parent, Type type, bool active = true) : _parent(parent), _type(type), _active(active) {}
	void setActive(bool value) { _active = value; };

	virtual bool init() { return true; }
	virtual bool start() { return true; }
	virtual UpdateStatus preUpdate() { return UpdateStatus::Continue; }
	virtual UpdateStatus update(float dt = 0.0f) { return UpdateStatus::Continue; }
	virtual UpdateStatus postUpdate() { return UpdateStatus::Continue; }
	virtual bool cleanUp() { return true; }

	virtual void OnEditor() = 0;

private:
	GameObject& _parent; 
	const Type _type;
	bool _active = true;
};