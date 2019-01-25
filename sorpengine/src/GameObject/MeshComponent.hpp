#pragma once

#include "Component.hpp"

class Mesh;
class GameObject;

class MeshComponent : public Component
{
    MeshComponent(Mesh* mesh = nullptr);

    UpdateStatus update(float dt = 0.0f) override;

    void OnEditor() override;

private:
    Mesh* _mesh = nullptr;
};
