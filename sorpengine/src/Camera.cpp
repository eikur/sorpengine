#include "Camera.hpp"
#include "Application.hpp"
#include "ModuleWindow.hpp"

void Camera::Init(const float aspectRatio)
{
    SetAspectRatio(aspectRatio);
	// by default perspective frustum
	SetFOV(_verticalFOV);
	SetPlaneDistances(_nearPlaneDistance, _farPlaneDistance);

	_frustum.SetFrame(float3::zero, -float3::unitZ, float3::unitY);
	_frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	// calculate values for orthogonal projection
	_orthoHeight = _nearPlaneDistance * math::Abs(_verticalFOV * 0.5f);
	_orthoWidth = _aspectRatio * _orthoHeight;
}

void Camera::switchType()
{
	if (_frustum.Type() == FrustumType::PerspectiveFrustum)
	{
		_frustum.SetOrthographic(_orthoWidth, _orthoHeight);
	}
	else
	{
		SetFOV(_verticalFOV);
	}
}

const float4x4& Camera::GetProjectionMatrix() const
{
	float4x4 rowMajorProjectionMatrix = _frustum.ProjectionMatrix();
	return rowMajorProjectionMatrix.Transposed();
}

const float4x4& Camera::GetViewMatrix() const
{
	float4x4 rowMajorViewMatrix = _frustum.ViewMatrix();
	return rowMajorViewMatrix.Transposed();
}

void Camera::SetFOV(float verticalFOV)
{
	_verticalFOV = verticalFOV;
	_frustum.SetVerticalFovAndAspectRatio(DegToRad(verticalFOV), _aspectRatio);
}

void Camera::SetAspectRatio(float aspectRatio)
{
 	_aspectRatio = aspectRatio;
	if (_frustum.Type() == FrustumType::PerspectiveFrustum)
	{
		float horizontalFOV = 2.0f * atanf(tanf(_frustum.VerticalFov() / 2.0f) * aspectRatio);
		_frustum.SetHorizontalFovAndAspectRatio(horizontalFOV, aspectRatio);
	}
	else
	{
		_orthoWidth = _aspectRatio * _orthoHeight;
		_frustum.SetOrthographic(_orthoWidth, _orthoHeight);
	}
}

void Camera::SetPlaneDistances(float nearPlane, float farPlane)
{
	_nearPlaneDistance = nearPlane;
	_farPlaneDistance = farPlane;
	_frustum.SetViewPlaneDistances(_nearPlaneDistance, _farPlaneDistance);
}

const float3& Camera::GetPosition() const
{
	return _frustum.Pos();
}

void Camera::SetPosition(const float3& newPosition)
{
	_frustum.SetPos(newPosition);
}

void Camera::Orientate(const float3& front, const float3& up)
{
	_frustum.SetFrame(_frustum.Pos(), front, up);
}

void Camera::LookAt(const float3& lookAtPosition)
{
	// check for bugs
	float3 newFront = (lookAtPosition - _frustum.Pos()).Normalized();
	float3 translation = newFront - _frustum.Front();
	Orientate(newFront, _frustum.Up() + translation);
}