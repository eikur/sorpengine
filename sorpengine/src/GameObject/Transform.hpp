#pragma once

#include "Component.hpp"
#include "MathGeoLib.h"

class Transform : public Component
{
public:
	Transform(const float3 position, const float rotation, const float2 scale);

	bool init() override;	
	UpdateStatus update(float dt = 0.0f) override;

	const float3 getPosition() const;
	const float getRotation() const;
	const float2 getScale() const;
	float4x4 getTransformMatrix() const;

	void setPosition(const float2 position);
	void setPosition(const float3 position);
	void setZPosition(const float zPos);
	void setRotation(const float rotation);
	void setScale(const float2 scale);

	void translate(const float3 translation);
	void rotateBy(const float rotation);
	void scaleBy(const float2 scale);

	void OnEditor() override;

protected:
	void setActive(const bool value) override;
	void showPosGizmo() const;

private:
	float3 _position = float3::zero;
	float  _rotation = 0.f;
	float2 _scale = float2::one;

	bool _showPosGizmo = true;
};
