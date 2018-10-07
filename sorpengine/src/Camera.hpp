#pragma once

#include "GameObject\GameObject.hpp"
#include "MathGeoLib.h"

class Camera
{
public:
	void Init();

	const float4x4& GetProjectionMatrix() const;
	const float4x4& GetViewMatrix() const;
	void SetFOV(float verticalFOV);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float nearPlane, float farPlane);
	const float3& GetPosition() const;
	void SetPosition(const float3& pos);
	void Orientate(const float3& front, const float3& up);
	void LookAt(const float3& lookAtPosition);

private:
	Frustum _frustum;
	float _nearPlaneDistance = 0.1f;
	float _farPlaneDistance = 100.0f;
	float _verticalFOV = 60.0f;
	float _aspectRatio = 1.78f;
};