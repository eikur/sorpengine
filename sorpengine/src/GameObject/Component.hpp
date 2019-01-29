#pragma once
#include <algorithm>
#include "Globals.hpp"

class GameObject;

enum class ComponentType
{
    Animation,
    Camera,
    Image,
	Material,
    Mesh,
    Transform,
    Script
};

class Component
{
  public:
	Component(ComponentType type, bool active = true) : _type(type), _active(active) {}

	virtual bool init() { return true; }
	virtual bool start() { return true; }
	virtual UpdateStatus preUpdate() { return UpdateStatus::Continue; }
	virtual UpdateStatus update(float dt = 0.0f) { return UpdateStatus::Continue; }
	virtual UpdateStatus postUpdate() { return UpdateStatus::Continue; }
	virtual bool cleanUp() { return true; }

    virtual void onEditor() = 0;

	void setParent(GameObject& parent) { _parent = &parent; }

protected:
	virtual void setActive(const bool value) { _active = value; };
	bool _active = true;

private:
	const ComponentType _type;
	GameObject * _parent = nullptr;
};