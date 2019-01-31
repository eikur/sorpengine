#include "MeshComponent.hpp"

#include "GameObject.hpp"
#include "ImGui/imgui.h"
#include "Mesh.hpp"
#include <map>

MeshComponent::MeshComponent(Mesh* mesh)
    : Component(ComponentType::Mesh, mesh != nullptr)
    , _mesh(mesh)
{

}

bool MeshComponent::init()
{
    tryToSkinMesh();
    return true;
}

UpdateStatus MeshComponent::update(float dt)
{
    if (!_active)
    {
        return UpdateStatus::Continue;
    }

    if (_mesh)
    {
        const bool gameTimeIsRunning = dt > 0.f;
        if (gameTimeIsRunning)
        {
            _mesh->updateSkin();
        }
        _mesh->draw();
    }

    return UpdateStatus::Continue;
}

void MeshComponent::onEditor()
{
    if (ImGui::CollapsingHeader("Mesh"))
    {
        ImGui::Checkbox("Enabled", &_active);
        // do something else! :P
    }
}

void MeshComponent::tryToSkinMesh()
{
    if (!_mesh || !_mesh->canBeSkinned() || _mesh->isSkinned())
    {
        return;
    }

    using BoneIdToGameObject = std::map<int, GameObject*>;
    BoneIdToGameObject boneTargets;

    const GameObject* rootSearchGameObject = _parent->findModelRoot();

    const std::vector<std::string> boneNames = _mesh->getBoneNames();
    const size_t boneCount = boneNames.size();

    for (size_t i = 0; i < boneCount; ++i)
    {
        const std::string& nameToSearch = boneNames.at(i);
        GameObject* go = rootSearchGameObject->findChildRecursivelyByName(nameToSearch);
        if (go != nullptr)
        {
            boneTargets[i] = go;
        }
    }

    _mesh->skin(boneTargets);
}