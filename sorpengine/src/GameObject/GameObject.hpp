#pragma once
#include <vector>
#include <algorithm>
#include "Component.hpp"
#include "Globals.hpp"
#include "MathGeoLib.h"
#include "ImGui/imgui.h"


class Transform;

class GameObject
{
public:
	GameObject(const std::string& name, GameObject *parent = nullptr, bool active = true);
	void setActive(bool value);
	const std::string& getName() const;

	void setParent(GameObject* parent);
    GameObject* getParent() const;

    void addChild(std::unique_ptr<GameObject>&& child);
	void removeChild(GameObject* child);
	bool removeFromParentAndCleanup();

	void addComponent(std::unique_ptr<Component>&& component);

	bool init();
	bool start();
	UpdateStatus preUpdate();
	UpdateStatus update(float dt = 0.0f);
	UpdateStatus postUpdate();
	bool cleanUp();

	void onEditor();
	void onHierarchy(int& index, ImGuiTreeNodeFlags nodeFlags, GameObject*& selectedGameObject);

    float4x4 GetWorldTransformMatrix() const;
    float4x4 GetModelSpaceTransformMatrix() const;
    float4x4 GetLocalTransformMatrix() const;

    GameObject* findChildRecursivelyByName(const std::string& name) const;
    void updateTransform(const float3& position, const Quat& rotation);

    void markAsModelRoot();
    bool isModelRoot() const;
    const GameObject* findModelRoot() const;

private:
	GameObject* findChild(GameObject* child) const;

private:
	std::vector<std::unique_ptr<GameObject>> _children;
	std::vector<std::unique_ptr<Component>> _components;

    GameObject* _parent = nullptr;
    Transform* _transform = nullptr;

	bool _active = true;
	std::string _name;

    bool _modelRoot = false;
};