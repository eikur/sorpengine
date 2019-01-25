#pragma once

#include "GameObject\GameObject.hpp"
#include "MathGeoLib.h"

class Camera
{
public:
    void Init(const float aspectRatio);

	const float4x4& getProjectionMatrix() const;
	const float4x4& GetViewMatrix() const;
    void switchType();

	void setFOV(float verticalFOV);
	void setAspectRatio(float aspectRatio);
	void setPlaneDistances(float nearPlane, float farPlane);

	const float3& getPosition() const;
	void setPosition(const float3& pos);
    void translate(const float3& translation);

	void orientate(const float3& front, const float3& up);
	void lookAt(const float3& lookAtPosition);

    const float3& up() const;
    const float3& front() const;
    const float3& right() const;

private:
	Frustum _frustum;

	float _nearPlaneDistance = 0.1f;
	float _farPlaneDistance = 1000.0f;
	float _verticalFOV = 60.0f;
	float _aspectRatio = 1.78f;
	float _orthoHeight = 0.0f;
	float _orthoWidth = 0.0f;
};