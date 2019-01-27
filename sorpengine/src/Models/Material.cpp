#include "Material.hpp"

Material::Material(const GLuint textureId)
{}


void Material::setColor(const float4& color, const ColorComponent component)
{
	_color[component] = color;
}

const GLfloat* Material::getColor(const ColorComponent component) const
{
	return (GLfloat*)&_color[component];
}

const GLuint Material::getTextureId() const
{
	return _textureId;
}