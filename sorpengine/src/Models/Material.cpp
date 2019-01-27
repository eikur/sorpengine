#include "Material.hpp"

Material::Material(const int textureId)
	: _textureId(textureId)
{}

void Material::setColor(const float4& color, const ColorComponent component)
{
	_color[component] = color;
}

GLfloat* Material::getColor(const ColorComponent component) const
{
	return (GLfloat*)&_color[component];
}

int Material::getTextureId() const
{
	return _textureId;
}

void Material::setTextureId(const int textureId)
{
	_textureId = textureId;
}