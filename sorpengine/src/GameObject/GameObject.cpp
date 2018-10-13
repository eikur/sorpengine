#include "GameObject.hpp"
#include "Component.hpp"
#include "Transform.hpp"
#include "SDL_opengl.h"

GameObject::GameObject(const std::string& name, GameObject* parent, bool active)
	: _name(name), _parent(parent), _active(active)
{
	addComponent(new Transform(*this, active));
	_transform = static_cast<Transform*>(_components.front());
}

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

bool GameObject::removeFromParentAndCleanup()
{
	if (_parent != nullptr)
	{
		_parent->removeChild(this);
	}
	return cleanUp();
}

void GameObject::addChild(GameObject* child)
{
	_children.push_back(child);
}

void GameObject::removeChild(GameObject* child)
{
	std::remove_if(_children.begin(), _children.end(), [child](const GameObject* obj) { return obj == child; });
}

void GameObject::addComponent(Component* component)
{
	_components.push_back(component);
}

void GameObject::removeComponent(Component* component)
{
	std::remove_if(_components.begin(), _components.end(), [component](Component* c) {return component == c; });
}

GameObject* GameObject::findChild(GameObject* child) const
{
	return *std::find_if(_children.begin(), _children.end(), [child](const GameObject* obj) { return obj == child; });
}

UpdateStatus GameObject::preUpdate()
{
	if (!_active)
	{
		return UpdateStatus::Continue;
	}

	UpdateStatus status = UpdateStatus::Continue;
	for (std::size_t i = 0; i < _components.size() && status == UpdateStatus::Continue; ++i)
	{
		status = _components.at(i)->preUpdate();
	}
	for (std::size_t i = 0; i < _children.size() && status == UpdateStatus::Continue; ++i)
	{
		status = _children.at(i)->preUpdate();
	}

	return status;
}

UpdateStatus GameObject::update(float dt)
{
	if (!_active)
	{
		return UpdateStatus::Continue;
	}

	glPushMatrix();
	UpdateStatus status = UpdateStatus::Continue;
	for (std::size_t i = 0; i < _components.size() && status == UpdateStatus::Continue; ++i)
	{
		status = _components.at(i)->update(dt);
	}
	for (std::size_t i = 0; i < _children.size() && status == UpdateStatus::Continue; ++i)
	{
		status = _children.at(i)->update(dt);	 
	}
	glPopMatrix();

	return status;
}

UpdateStatus GameObject::postUpdate()
{
	if (!_active)
	{
		return UpdateStatus::Continue;
	}

	UpdateStatus status = UpdateStatus::Continue;
	for (std::size_t i = 0; i < _components.size() && status == UpdateStatus::Continue; ++i)
	{
		status = _components.at(i)->postUpdate();
	}
	for (std::size_t i = 0; i < _children.size() && status == UpdateStatus::Continue; ++i)
	{
		status = _children.at(i)->postUpdate();
	}

	return status;
}

bool GameObject::init()
{
	bool status = true;
	for (std::size_t i = 0; i < _components.size() && status; ++i)
	{
		status = status && _components.at(i)->init();
	}
	return status;
}

bool GameObject::start()
{
	bool status = true;
	for (std::size_t i = 0; i < _components.size() && status; ++i)
	{
		status = status && _components.at(i)->start();
	}
	return status;
}

bool GameObject::cleanUp()
{
	for (GameObject* child : _children)
	{
		child->cleanUp();
		delete child;
	}
	for (Component* component : _components)
	{
		component->cleanUp();
		delete component;
	}

	return true;
}

const float3 GameObject::getPosition() const
{
	return _transform->getPosition();
}

void GameObject::setPosition(const float3 position)
{
	_transform->setPosition(position);
}

void GameObject::translate(const float3 position)
{
	_transform->translate(position);
}

void GameObject::onEditor()
{
	ImGui::Checkbox(_name.c_str(), &_active);

	for (std::vector<Component*>::iterator it = _components.begin(); it != _components.end(); ++it)
	{
		(*it)->OnEditor();
	}
}

void GameObject::onHierarchy(int& index, ImGuiTreeNodeFlags nodeFlags, GameObject *& selectedGameObject)
{
	if (_children.empty())
	{
		ImGui::TreeNodeEx((void*)(intptr_t)index, nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", _name.c_str());
		if (ImGui::IsItemClicked())
		{
			selectedGameObject = this;
		}
		return;
	}

	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)index, 0, "%s", _name.c_str(), index);
	if (ImGui::IsItemClicked())
	{
		selectedGameObject = this;
	}

	if (node_open)
	{
		for (GameObject* child : _children)
		{
			child->onHierarchy(++index, nodeFlags, selectedGameObject);
		}
		ImGui::TreePop();
	}

}