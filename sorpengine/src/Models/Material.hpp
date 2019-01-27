#pragma once

#include "MathGeoLib/include/MathGeoLib.h"
#include "glew-2.0.0/include/GL/glew.h"

class Material
{
  public:
	enum ColorComponent
	{
		Ambient, 
		Diffuse,
		Emissive,
		Specular,
		Transparent
	};

	Material(const GLuint textureId);

	void setColor(const float4& color, const ColorComponent component);
	const GLfloat* getColor(const ColorComponent component) const;

	const GLuint getTextureId() const;

  private:
	  GLuint _textureId = 0;
	  float4 _color[5] = { float4::zero, float4::zero, float4::zero, float4::zero, float4::zero };
};
