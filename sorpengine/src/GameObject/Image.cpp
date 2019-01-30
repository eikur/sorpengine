#include "Image.hpp"

#include "Application.hpp"
#include "ImGui/imgui.h"
#include "TextureHelper.hpp"

namespace
{
	const std::string kDefaultTextureName = "resources/buu.png";
}

Image::Image() : Component(ComponentType::Image, true), _textureName(kDefaultTextureName)
{
}

Image::Image(const std::string& textureName) : Component(ComponentType::Image, true), _textureName(textureName)
{
}

Image::~Image()
{
	App->getTextureHelper().unloadTexture(_textureId);
}

bool Image::init()
{
	_textureId = App->getTextureHelper().loadTexture(_textureName);
	return true;
}

void Image::setAnchor(const float2& newAnchor)
{
	_anchor = newAnchor;
}

UpdateStatus Image::update(float)
{
	if (!_active)
	{
		return UpdateStatus::Continue;
	}

	TextureHelper& textures = App->getTextureHelper();
	textures.useTexture(_textureId);
	
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-_anchor.x, -_anchor.y);
	glTexCoord2f(1, 0);
    glVertex2f(1.f - _anchor.x, -_anchor.y + _skew.y);
	glTexCoord2f(1, 1);
    glVertex2f(1.f - _anchor.x + _skew.x, 1.f - _anchor.y + _skew.y);
	glTexCoord2f(0, 1);
    glVertex2f(-_anchor.x + _skew.x, 1.f - _anchor.y);
	glEnd();

	textures.stopUsingTexture();

	return UpdateStatus::Continue;
}

void Image::onEditor()
{
	if (ImGui::CollapsingHeader("Image"))
	{
        ImGui::InputInt("Texture", &_textureId, 1, 1);
        if (ImGui::CollapsingHeader("Texture Viewer", ImGuiTreeNodeFlags_Bullet || ImGuiTreeNodeFlags_AllowOverlapMode))
        {
            ImGui::Image((void*)_textureId, ImVec2(192.f, 192.f));
        }

		ImGui::DragFloat2("Anchor", (float*)&_anchor, 0.02f);
        ImGui::DragFloat2("Skew", (float*)&_skew, 0.02f);
		ImGui::Checkbox("Visible", &_active);
	}
}