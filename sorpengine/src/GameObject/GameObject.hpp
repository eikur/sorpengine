#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include "Component.hpp"
#include "Globals.hpp"
#include "MathGeoLib.h"
#include "ImGui/imgui.h"


class Transform;

class GameObject
{
public:
	GameObject(const std::string& name, GameObject *parent = nullptr, bool active = true);

	bool init();
	bool start();
	UpdateStatus preUpdate();
	UpdateStatus update(float dt = 0.0f);
	UpdateStatus postUpdate();
	bool cleanUp();

	const std::string& getName() const;
    GameObject* getParent() const;

    void setActive(bool value);
	void setParent(GameObject* parent);

    GameObject* findChildRecursivelyByName(const std::string& name) const;
    void addChild(std::unique_ptr<GameObject>&& child);
	void addComponent(std::unique_ptr<Component>&& component);

    void removeChild(GameObject* child);
    void markChildToDelete(GameObject* child);

    void updateTransform(const float3& position, const Quat& rotation);
    float4x4 GetWorldTransformMatrix() const;
    float4x4 GetModelSpaceTransformMatrix() const;
    float4x4 GetLocalTransformMatrix() const;
    
    const GameObject* findModelRoot() const;
    void markAsModelRoot();
    bool isModelRoot() const;

	void onEditor(std::function<void()> removeCallback = nullptr);
	void onHierarchy(int& index, ImGuiTreeNodeFlags nodeFlags, GameObject *&selectedGameObject);

private:
	GameObject* findChild(GameObject* child) const;

private:
	std::vector<std::unique_ptr<GameObject>> _children;
	std::vector<std::unique_ptr<Component>> _components;
    std::vector<GameObject*> _childrenToDelete;

    GameObject* _parent = nullptr;
    Transform* _transform = nullptr;
    

	bool _active = true;
	std::string _name;

    bool _modelRoot = false;
};