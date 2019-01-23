#pragma once

#include "Component.hpp"
#include "MathGeoLib.h"

class Image : public Component
{
  public:
	Image();
	Image(const std::string& textureName);

	~Image();

	bool init() override;
	UpdateStatus update(float dt = 0.0f) override;

	void setAnchor(const float2& newAnchor);
	void setSize(const float2& newSize);
	const float2& getSize() const;
	void OnEditor() override;

  private:
	  const std::string _textureName;
	  int _textureId = 0;
	  float2 _size = float2(1.f, 1.f);
	  
      float2 _anchor = float2(0.f, 0.f);
      float2 _anchorCorrection = float2(0.f, 0.f);
};
