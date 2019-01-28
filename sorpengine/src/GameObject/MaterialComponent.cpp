#include "MaterialComponent.hpp"

#include "Material.hpp"
#include "ImGui/imgui.h"

MaterialComponent::MaterialComponent(Material* material)
	: Component(ComponentType::Material, true)
	, _material(material)
{}

void MaterialComponent::onEditor()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		int textureId = _material->getTextureId();
		ImGui::InputInt("Texture", &textureId, 1, 1);
		if (ImGui::CollapsingHeader("Texture Viewer", ImGuiTreeNodeFlags_Bullet || ImGuiTreeNodeFlags_AllowOverlapMode))
		{
			ImGui::Image((void*)textureId, ImVec2(192.f, 192.f));
		}

		ImGui::DragFloat4("Ambient", _material->getColor(Material::ColorComponent::Ambient), 0.01f, 0.f, 1.f);
		ImGui::DragFloat4("Diffuse", _material->getColor(Material::ColorComponent::Diffuse), 0.01f, 0.f, 1.f);
		ImGui::DragFloat4("Emissive", _material->getColor(Material::ColorComponent::Emissive), 0.01f, 0.f, 1.f);
		ImGui::DragFloat4("Specular", _material->getColor(Material::ColorComponent::Specular), 0.01f, 0.f, 1.f);
		ImGui::DragFloat4("Transparent", _material->getColor(Material::ColorComponent::Transparent), 0.01f, 0.f, 1.f);

		// add more properties below

		_material->setTextureId(textureId);
	}
}