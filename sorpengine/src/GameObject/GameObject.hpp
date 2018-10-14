#pragma once
#include <vector>
#include <algorithm>
#include "Globals.hpp"
#include "MathGeoLib.h"
#include "ImGui/imgui.h"

class Component;
class Transform;

class GameObject
{
public:
	GameObject(const std::string& name, GameObject *parent = nullptr, bool active = true);
	void setActive(bool value);
	const std::string& getName() const;

	void setParent(GameObject* parent);
	void addChild(GameObject* child);
	void removeChild(GameObject* child);
	bool removeFromParentAndCleanup();

	void addTransform(std::shared_ptr<Transform>&& transform);
	void addComponent(std::shared_ptr<Component>&& component);

	bool init();
	bool start();
	UpdateStatus preUpdate();
	UpdateStatus update(float dt = 0.0f);
	UpdateStatus postUpdate();
	bool cleanUp();

	void onEditor();
	void onHierarchy(int& index, ImGuiTreeNodeFlags nodeFlags, GameObject*& selectedGameObject);

private:
	GameObject* findChild(GameObject* child) const;

private:
	GameObject* _parent = nullptr;
	std::vector<GameObject*> _children;
	std::vector<std::shared_ptr<Component>> _components;
	std::weak_ptr<Transform> _transform;

	bool _active = true;
	std::string _name;
};