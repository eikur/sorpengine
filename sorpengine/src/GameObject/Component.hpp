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
	Component(Type type, bool active = true) : _type(type), _active(active) {}

	virtual bool init() { return true; }
	virtual bool start() { return true; }
	virtual UpdateStatus preUpdate() { return UpdateStatus::Continue; }
	virtual UpdateStatus update(float dt = 0.0f) { return UpdateStatus::Continue; }
	virtual UpdateStatus postUpdate() { return UpdateStatus::Continue; }
	virtual bool cleanUp() { return true; }

	void setParent(GameObject& parent) { _parent = &parent; }
	virtual void OnEditor() = 0;

protected:
	virtual void setActive(const bool value) { _active = value; };

private:
	const Type _type;
	bool _active = true;
	GameObject * _parent = nullptr;
};