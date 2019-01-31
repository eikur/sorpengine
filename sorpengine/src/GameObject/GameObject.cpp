#include "GameObject.hpp"
#include "Component.hpp"
#include "TransformBuilder.hpp"
#include "SDL_opengl.h"

GameObject::GameObject(const std::string& name, GameObject* parent, bool active)
	: _name(name), _parent(parent), _active(active)
{
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
	_parent = parent;
}

GameObject* GameObject::getParent() const
{
    return _parent;
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
	child->setParent(this);
	_children.push_back(child);
}

void GameObject::removeChild(GameObject* child)
{
	std::remove_if(_children.begin(), _children.end(), [child](const GameObject* obj) { return obj == child; });
}

void GameObject::addTransform(std::shared_ptr<Transform>&& transform)
{
	_transform = transform;
	addComponent(std::move(transform));
}

void GameObject::addComponent(std::shared_ptr<Component>&& component)
{
	component->setParent(*this);
	_components.push_back(std::move(component));
}

GameObject* GameObject::findChild(GameObject* child) const
{
	return *std::find_if(_children.begin(), _children.end(), [child](const GameObject* obj) { return obj == child; });
}

GameObject* GameObject::findChildRecursivelyByName(const std::string& name) const
{
    GameObject* found = nullptr;

    auto it = std::find_if(_children.begin(), _children.end(), [name](const GameObject* obj) { return obj->getName() == name; });
    if (it != _children.end())
    {
        found = *it;
    }

    auto childrenIt = _children.begin();
    while (!found && childrenIt != _children.end())
    {
        found = (*childrenIt)->findChildRecursivelyByName(name);
        ++childrenIt;
    }

    return found;
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
	for (std::size_t i = 0; i < _children.size() && status; ++i)
	{
		status = _children.at(i)->init();
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
	for (auto&& component : _components)
	{
		component->cleanUp();
		component.reset();
	}

	return true;
}

void GameObject::onEditor()
{
	if (_parent == nullptr)
	{
		return;
	}

	ImGui::Checkbox(_name.c_str(), &_active);
    ImGui::SameLine();
    ImGui::Checkbox("Model Root", &_modelRoot);

	for (auto& component : _components)
	{
		component->onEditor();
	}
}

void GameObject::onHierarchy(int& index, ImGuiTreeNodeFlags nodeFlags, GameObject *& selectedGameObject) 
{
    const bool isSceneRootNode = _parent == nullptr;
    
    if (isSceneRootNode)
    {
        for (GameObject* child : _children)
        {
            child->onHierarchy(++index, nodeFlags, selectedGameObject);
        }
        return;
    }


    if (_children.empty())
	{
		ImGui::TreeNodeEx((void*)(intptr_t)index, nodeFlags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, "%s", _name.c_str());
		if (ImGui::IsItemClicked())
		{
			selectedGameObject = this;
		}
		return;
	}

	const bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)index, 0, "%s", _name.c_str(), index);
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

float4x4 GameObject::GetWorldTransformMatrix() const
{
    if (_parent == nullptr)
    {
        return GetLocalTransformMatrix(); 
    }

    return _parent->GetWorldTransformMatrix() * GetLocalTransformMatrix();
}

float4x4 GameObject::GetModelSpaceTransformMatrix() const
{
    if (_parent == nullptr || isModelRoot())
    {
        return float4x4::identity; 
    }

    return _parent->GetModelSpaceTransformMatrix() * GetLocalTransformMatrix();
}

float4x4 GameObject::GetLocalTransformMatrix() const
{
    return _transform.lock()->getTransformMatrix();
}


void GameObject::updateTransform(const float3& position, const Quat& rotation)
{
    Transform& t = *_transform.lock();
    t.setPosition(position);
    t.setRotation(rotation);
}

void GameObject::markAsModelRoot()
{
    _modelRoot = true;
}

bool GameObject::isModelRoot() const
{
    return _modelRoot;
}

const GameObject* GameObject::findModelRoot() const
{
    if (_parent == nullptr)
    {
        return this;
    }
    return _modelRoot ? this : _parent->findModelRoot();
}