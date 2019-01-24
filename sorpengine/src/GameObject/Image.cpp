#include "Image.hpp"

#include "Application.hpp"
#include "ImGui/imgui.h"
#include "TextureHelper.hpp"

namespace
{
	const std::string kDefaultTextureName = "resources/buu.png";
}

Image::Image() : Component(Component::Type::Image, true), _textureName(kDefaultTextureName)
{
}

Image::Image(const std::string& textureName) : Component(Component::Type::Image, true), _textureName(textureName)
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
	glVertex2f(1.f - _anchor.x, -_anchor.y);
	glTexCoord2f(1, 1);
	glVertex2f(1.f - _anchor.x, 1.f - _anchor.y);
	glTexCoord2f(0, 1);
	glVertex2f(-_anchor.x, 1.f - _anchor.y);
	glEnd();

	textures.stopUsingTexture();

	return UpdateStatus::Continue;
}

void Image::OnEditor()
{
	if (ImGui::CollapsingHeader("Image"))
	{
		ImGui::DragFloat2("Anchor", (float*)&_anchor, 0.02f);
		ImGui::Checkbox("Visible", &_active);
	}
}