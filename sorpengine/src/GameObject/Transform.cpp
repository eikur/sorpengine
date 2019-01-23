#include "Transform.hpp"
#include "ImGui/imgui.h"
#include "GL/glew.h"

Transform::Transform(const float3 position, const float rotation, const float2 scale)
	: Component(Component::Type::Transform, true)
	, _position(position)
	, _rotation(rotation)
	, _scale(scale)
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
	glRotatef(_rotation, 0.f, 0.f, 1.f);
	showPosGizmo();
	glScalef(_scale.x, _scale.y, 1.f);

	return UpdateStatus::Continue;
}

// -- Getters
const float3 Transform::getPosition() const { return _position; }
const float Transform::getRotation() const { return _rotation; }
const float2 Transform::getScale() const { return _scale; }

float4x4 Transform::getTransformMatrix() const
{
	Quat rotationQuat = Quat::FromEulerXYX(0.f, 0.f, _rotation);
	return float4x4::FromTRS(_position, rotationQuat, float3(_scale, 1.f));
}

// -- Setters
void Transform::setPosition(const float2 position)
{
	_position = float3(position, 0.f);
}

void Transform::setZPosition(const float zPos)
{
	_position.z = zPos;
}

void Transform::setPosition(const float3 position)
{
	_position = position;
}

void Transform::translate(const float3 translation)
{
	const float3 finalPos = _position + translation;
	setPosition(finalPos);
}

void Transform::setRotation(const float rotation)
{
	_rotation = rotation;
}

void Transform::rotateBy(const float rotation)
{
	_rotation = fmod(_rotation + rotation, 360.f);
}

void Transform::setScale(const float2 scale)
{
	_scale = scale;
}

void Transform::scaleBy(const float2 scale)
{
	_scale.x *= scale.x;
	_scale.y *= scale.y;
}

// ----
void Transform::OnEditor()
{
	if (ImGui::CollapsingHeader("Local Transform"))
	{
		float pos[3] = { _position.x, _position.y, _position.z };
		float scl[2] = { _scale.x, _scale.y};
		float rot = _rotation;
		
		ImGui::DragFloat3("Position", pos, 0.05f);
		ImGui::DragFloat("Rotation", &rot, 0.5f);
		ImGui::DragFloat2("Scale", scl, 0.05f);

		_position = { pos[0], pos[1], pos[2] };
		_scale = { scl[0], scl[1] };
		_rotation = rot;

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
	glColor3f(1.f, 1.f, 1.f);
}