#pragma once

#include "Component.hpp"

class Mesh;
class GameObject;

class MeshComponent : public Component
{
  public:
    MeshComponent(Mesh* mesh = nullptr);

    // Component methods
    bool init() override; 
    UpdateStatus update(float dt = 0.0f) override;
    void onEditor() override;

  private:
      void tryToSkinMesh();

    Mesh* _mesh = nullptr;
};
