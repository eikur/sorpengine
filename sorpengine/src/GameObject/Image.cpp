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
    _anchorCorrection = _anchor.Mul(_size);
}

void Image::setSize(const float2& newSize)
{
	_size = newSize;
}

const float2& Image::getSize() const
{
	return _size;
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
	glVertex2f(-_anchorCorrection.x, -_anchorCorrection.y);
	glTexCoord2f(1, 0);
	glVertex2f(_size.x - _anchorCorrection.x, -_anchorCorrection.y);
	glTexCoord2f(1, 1);
	glVertex2f(_size.x - _anchorCorrection.x, _size.y - _anchorCorrection.y);
	glTexCoord2f(0, 1);
	glVertex2f(- _anchorCorrection.x, _size.y - _anchorCorrection.y);
	glEnd();

	textures.stopUsingTexture();

	return UpdateStatus::Continue;
}

void Image::OnEditor()
{
	if (ImGui::CollapsingHeader("Image"))
	{
		float size[2] = { _size.x, _size.y };
		float anchor[2] = { _anchor.x, _anchor.y };

		ImGui::DragFloat2("Size", size, 0.05f);
		ImGui::DragFloat2("Anchor", anchor, 0.02f);

        setSize({ size[0], size[1] });
        setAnchor({ anchor[0], anchor[1] });

		ImGui::Checkbox("Active", &_active);
	}
}