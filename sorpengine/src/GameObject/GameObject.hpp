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

	void addComponent(Component* component);
	void removeComponent(Component* component);

	bool init();
	bool start();
	UpdateStatus preUpdate();
	UpdateStatus update(float dt = 0.0f);
	UpdateStatus postUpdate();
	bool cleanUp();

	const float3 getPosition() const;
	void setPosition(const float3 position);
	void translate(const float3 position);

	void onEditor();
	void onHierarchy(int& index, ImGuiTreeNodeFlags nodeFlags, GameObject*& selectedGameObject);

private:
	GameObject* findChild(GameObject* child) const;

private:
	GameObject* _parent = nullptr;
	std::vector<GameObject*> _children;
	std::vector<Component*> _components;
	Transform* _transform = nullptr;

	bool _active = true;
	std::string _name;
};