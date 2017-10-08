#include "GameObject.h"

GameObject::GameObject(const std::string& name, GameObject* parent, bool active)
	: _name(name), _parent(parent), _active(active)
{}

void GameObject::setActive(bool value)
{
	_active = value;
}

const std::string& GameObject::getName() const
{
	return _name;
}

void GameObject::setParent(GameObject* parent)
{
	if (_parent != nullptr)
	{
		_parent->removeChild(this);
	}
	if (parent != nullptr)
	{
		_parent->addChild(this);
	}
	_parent = parent;
}

void GameObject::addChild(GameObject* child)
{
	_children.push_back(child);
}

void GameObject::removeChild(GameObject* child)
{
	std::remove_if(_children.begin(), _children.end(), [child](const GameObject* obj) { return obj == child; });
}

GameObject* GameObject::findChild(GameObject* child) const
{
	return *std::find_if(_children.begin(), _children.end(), [child](const GameObject* obj) { return obj == child; });
}

UpdateStatus GameObject::preUpdate()
{
	return UpdateStatus::Continue;
}

UpdateStatus GameObject::update(float)
{
	return UpdateStatus::Continue;
}

UpdateStatus GameObject::postUpdate()
{
	return UpdateStatus::Continue;
}

bool GameObject::init()
{
	return true;
}

bool GameObject::start()
{
	return true;
}

bool  GameObject::cleanUp()
{
	return true;
}
