#include "MeshComponent.hpp"

#include "ImGui/imgui.h"
#include "Mesh.hpp"

MeshComponent::MeshComponent(Mesh* mesh)
    : Component(ComponentType::Mesh, mesh != nullptr)
    , _mesh(mesh)
{

}

UpdateStatus MeshComponent::update(float /*dt*/)
{
    if (_active && _mesh)
    {
        _mesh->draw();
    }

    return UpdateStatus::Continue;
}

void MeshComponent::OnEditor()
{
    if (ImGui::CollapsingHeader("Mesh"))
    {
        ImGui::Checkbox("Enabled", &_active);
        // do something else! :P
    }
}