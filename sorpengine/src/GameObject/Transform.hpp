#pragma once

#include "Component.hpp"
#include "MathGeoLib.h"

class Transform : public Component
{
public:
	Transform(GameObject& parent, bool active = true);
	~Transform() = default;

	bool init() override;	// remove me
	UpdateStatus update(float dt = 0.0f) override;

	void setPosition(const float3 translation);
	void translate(const float3 translation);
	
	void setRotation(const Quat rotation);
	void rotate(const Quat rotation);

	void setScale(const float3 scale);

private:
	float3 _position = float3::zero;
	Quat   _rotation = Quat::identity;
	float3 _scale = float3::one; 

	// remove me
	int _textureId = 0;
};