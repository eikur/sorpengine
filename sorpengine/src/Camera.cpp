#include "Camera.hpp"

#include "Application.hpp"
#include "ModuleWindow.hpp"
#include "Utils.hpp"

void Camera::Init(const float aspectRatio)
{
    setAspectRatio(aspectRatio);
	// by default perspective frustum
	setFOV(_verticalFOV);
	setPlaneDistances(_nearPlaneDistance, _farPlaneDistance);

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
		setFOV(_verticalFOV);
	}
}

const float4x4& Camera::getProjectionMatrix() const
{
	float4x4 rowMajorProjectionMatrix = _frustum.ProjectionMatrix();
	return rowMajorProjectionMatrix.Transposed();
}

const float4x4& Camera::GetViewMatrix() const
{
	float4x4 rowMajorViewMatrix = _frustum.ViewMatrix();
	return rowMajorViewMatrix.Transposed();
}

void Camera::setFOV(float verticalFOV)
{
	_verticalFOV = verticalFOV;
	_frustum.SetVerticalFovAndAspectRatio(DegToRad(verticalFOV), _aspectRatio);
}

void Camera::setAspectRatio(float aspectRatio)
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

void Camera::setPlaneDistances(float nearPlane, float farPlane)
{
	_nearPlaneDistance = nearPlane;
	_farPlaneDistance = farPlane;
	_frustum.SetViewPlaneDistances(_nearPlaneDistance, _farPlaneDistance);
}

const float3& Camera::getPosition() const
{
	return _frustum.Pos();
}

void Camera::setPosition(const float3& newPosition)
{
	_frustum.SetPos(newPosition);
    Utils::log("camera pos (%.2f,%.2f,%.2f)", newPosition.x, newPosition.y, newPosition.z);
}

void Camera::translate(const float3& translation)
{
    setPosition(getPosition() + translation);
}

void Camera::orientate(const float3& front, const float3& up)
{
	_frustum.SetFrame(_frustum.Pos(), front, up);
}

void Camera::lookAt(const float3& lookAtPosition)
{
	// check for bugs
	float3 newFront = (lookAtPosition - _frustum.Pos()).Normalized();
	float3 translation = newFront - _frustum.Front();
	orientate(newFront, _frustum.Up() + translation);
}

const float3& Camera::up() const
{
    return _frustum.Up();
}

const float3& Camera::front() const
{
    return _frustum.Front();
}

const float3& Camera::right() const
{
    return _frustum.WorldRight();
}
