#pragma once
#include <list>
#include <algorithm>
#include "Globals.hpp"

class GameObject
{
public:
	GameObject(const std::string& name, GameObject *parent = nullptr, bool active = true);
	void setActive(bool value);
	const std::string& getName() const;

	void setParent(GameObject* parent);
	void addChild(GameObject* child);
	void removeChild(GameObject* child);

	bool init();
	bool start();
	UpdateStatus preUpdate();
	UpdateStatus update(float dt = 0.0f);
	UpdateStatus postUpdate();
	bool cleanUp();

private:
	GameObject* findChild(GameObject* child) const;

private:
	GameObject* _parent = nullptr;
	std::list<GameObject*> _children;
	bool _active = true;
	std::string _name;
};