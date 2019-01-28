#pragma once

#include "Component.hpp"
#include "MathGeoLib.h"

class Transform : public Component
{
public:
	Transform(const float3& position, const Quat& rotation, const float3& scale);

	UpdateStatus update(float dt = 0.0f) override;

	const float3& getPosition() const;
	const Quat& getRotation() const;
	const float3& getScale() const;
	float4x4 getTransformMatrix() const;

	void setPosition(const float2& position);
	void setPosition(const float3& position);
	void setZPosition(const float zPos);
	void setRotation(const Quat& rotation);
	void setScale(const float2& scale);
	void setScale(const float3& scale);

	void translate(const float3& translation);
	void rotateBy(const Quat& rotation);
	void scaleBy(const float2& scale);
	void scaleBy(const float3& scale);

	void onEditor() override;

protected:
	void setActive(const bool value) override;
	void showPosGizmo() const;

private:
	float3 _position = float3::zero;
	Quat  _rotation = Quat::identity;
	float3 _scale = float3::one;

	float3 _eulerRotation = float3::zero;

	bool _showPosGizmo = false;
};
