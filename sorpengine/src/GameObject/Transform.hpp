#pragma once

#include "Component.hpp"
#include "MathGeoLib.h"

class Transform : public Component
{
public:
	Transform(const float3 position, const Quat rotation, const float3 scale);

	bool init() override;	
	UpdateStatus update(float dt = 0.0f) override;

	const float3 getPosition() const;
	const Quat getRotation() const;
	const float3 getScale() const;
	float4x4 getTransformMatrix() const;

	void setPosition(const float3 translation);	
	void setRotation(const Quat rotation);
	void setScale(const float3 scale);
	void setTransform(const float3 position, const Quat rotation, const float3 scale);

	void translate(const float3 translation);
	void rotate(const Quat rotation);

	void OnEditor() override;

protected:
	void setActive(const bool value) override;

private:
	void recacheTransformIfNeeded();

	float3 _position = float3::zero;
	Quat   _rotation = Quat::identity;
	float3 _scale = float3::one;

	float3 _cachedPosition = float3::zero;
	Quat _cachedRotation = Quat::identity;
	float3 _cachedScale = float3::one;
	// remove me
	int _textureId = 0;
};