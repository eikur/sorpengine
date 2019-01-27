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

	Material(const int textureId);

	void setColor(const float4& color, const ColorComponent component);
	GLfloat* getColor(const ColorComponent component) const;

	int getTextureId() const;
	void setTextureId(const int textureId);

  private:
	  int _textureId = 0;
	  float4 _color[5] = { float4::zero, float4::zero, float4::zero, float4::zero, float4::zero };
};
