#include "Transform.hpp"
#include "ImGui/imgui.h"
#include "GL/glew.h"

Transform::Transform(const float3& position, const Quat& rotation, const float3& scale)
	: Component(ComponentType::Transform, true)
	, _position(position)
	, _rotation(rotation)
	, _scale(scale)
	, _eulerRotation(rotation.ToEulerXYZ() * 180.f / pi)
{
}

void Transform::setActive(const bool value)
{
	assert(false, "Transform component's setActive() should never be called");
}

bool Transform::init()
{
	return true;
}


// -- update methods
UpdateStatus Transform::update(float dt) 
{
	glTranslatef(_position.x, _position.y, _position.z);
	
	glRotatef(_eulerRotation.x, 1.f, 0.f, 0.f);
	glRotatef(_eulerRotation.y, 0.f, 1.f, 0.f);
	glRotatef(_eulerRotation.z, 0.f, 0.f, 1.f);
	
	showPosGizmo();
	
	glScalef(_scale.x, _scale.y, _scale.z);

	return UpdateStatus::Continue;
}

// -- Getters
const float3& Transform::getPosition() const { return _position; }
const Quat& Transform::getRotation() const { return _rotation; }
const float3& Transform::getScale() const { return _scale; }

float4x4 Transform::getTransformMatrix() const
{
	return float4x4::FromTRS(_position, _rotation, _scale);
}

// -- Setters
void Transform::setPosition(const float2& position)
{
	_position = float3(position, 0.f);
}

void Transform::setZPosition(const float zPos)
{
	_position.z = zPos;
}

void Transform::setPosition(const float3& position)
{
	_position = position;
}

void Transform::translate(const float3& translation)
{
	const float3 finalPos = _position + translation;
	setPosition(finalPos);
}

void Transform::setRotation(const Quat& rotation)
{
	_rotation = rotation;
}

void Transform::rotateBy(const Quat& rotation)
{
	_rotation = _rotation.Mul(rotation);
}

void Transform::setScale(const float2& scale)
{
	_scale = float3(scale, 1.f);
}

void Transform::setScale(const float3& scale)
{
	_scale = scale;
}

void Transform::scaleBy(const float2& scale)
{
	_scale.x *= scale.x;
	_scale.y *= scale.y;
}

void Transform::scaleBy(const float3& scale)
{
	_scale.x *= scale.x;
	_scale.y *= scale.y;
	_scale.z *= scale.z;
}

// ----
void Transform::OnEditor()
{
	if (ImGui::CollapsingHeader("Local Transform"))
	{
		ImGui::DragFloat3("Position", (float*)&_position, 0.05f);
		ImGui::DragFloat3("Rotation", (float*)&_eulerRotation, 0.2f);
		ImGui::DragFloat3("Scale", (float*)&_scale, 0.05f);

		_rotation = Quat::FromEulerXYZ(_eulerRotation.x * pi / 180.f, _eulerRotation.y * pi / 180.f, _eulerRotation.z * pi / 180.f);

		ImGui::Checkbox("Show Gizmo", &_showPosGizmo);
	}
}

void Transform::showPosGizmo() const
{
	if (!_showPosGizmo)
	{
		return;
	}
	//TODO modify depending on distance to the camera
	const float w = 0.1f;
	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_LINES);
	glVertex2f(0.f, 0.f);
	glVertex2f(w, 0.f);
	glEnd();
	glColor3f(0.f, 1.f, 0.f);
	glBegin(GL_LINES);
	glVertex2f(0.f, 0.f);
	glVertex2f(0.f, w);
	glEnd();
	glColor3f(0.f, 0.f, 1.f);
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f,w);
	glEnd();
	glColor3f(1.f, 1.f, 1.f);
}