#pragma once

#include "GameObject\Component.hpp"
#include "MathGeoLib.h"

class Camera : public Component
{
public:
    Camera(const bool active = true);

    // TODO unify this a bit :)
    bool init() override; // override from Component
    void Init(const float aspectRatio);

	float4x4 getProjectionMatrix() const;
	float4x4 getViewMatrix() const;
    void switchType();

	void setFOV(float verticalFOV);
	void setAspectRatio(float aspectRatio);
	void setPlaneDistances(float nearPlane, float farPlane);

    void updateFrustumTransform(const float4x4& worldTransformMatrix);

	const float3& getPosition() const;
	void setPosition(const float3& pos);
    void translate(const float3& translation);

	void orientate(const float3& front, const float3& up);
	void lookAt(const float3& lookAtPosition);

    const float3& up() const;
    const float3& front() const;
    const float3& right() const;

    const float4& getClearColor() const;

    void onEditor() override;

private:
	Frustum _frustum;

	float _nearPlaneDistance = 0.1f;
	float _farPlaneDistance = 1000.0f;
	float _verticalFOV = 60.0f;
	float _aspectRatio = 1.78f;
	float _orthoHeight = 0.0f;
	float _orthoWidth = 0.0f;

    float4 _clearColor = float4(0.1f, 0.1f, 0.1f, 1.f);
};