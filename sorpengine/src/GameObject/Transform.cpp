#include "Transform.hpp"
#include "ImGui/imgui.h"
// to remove
#include "Application.hpp"
#include "TextureHelper.hpp"

namespace
{
	// remove me
	const std::string kTextureName = "resources/Lenna.png";
}

Transform::Transform(GameObject& parent, bool active)
	: Component(parent, Component::Type::Transform, active)
{}

bool Transform::init()
{
	_textureId = App->getTextureHelper().loadTexture(kTextureName);
	_position.z = -0.8f;
	return true;
}


// -- update methods
UpdateStatus Transform::update(float dt)
{
	recacheTransformIfNeeded();

	glTranslatef(_position.x, _position.y, _position.z);
	float3 euler_rot = _rotation.ToEulerXYZ() * 180.0f / pi;
	glRotatef(euler_rot.x, 1, 0, 0);
	glRotatef(euler_rot.y, 0, 1, 0);
	glRotatef(euler_rot.z, 0, 0, 1);
	glScalef(_scale.x, _scale.y, _scale.z);

	// remove me
	float w = 0.1f;

	App->getTextureHelper().useTexture(_textureId);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(_position.x - w / 2, _position.y - w / 2, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(_position.x + w / 2, _position.y - w / 2, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(_position.x + w / 2, _position.y + w / 2, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(_position.x - w / 2, _position.y + w / 2, 0);
	glEnd();

	return UpdateStatus::Continue;
}

// -- transform modification methods
const float3 Transform::getPosition() const
{
	return _position;
}

const Quat Transform::getRotation() const
{
	return _rotation;
}

const float3 Transform::getScale() const
{
	return _scale;
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

void Transform::setRotation(const Quat rotation)
{
	_rotation = rotation;
}

void Transform::rotate(const Quat rotation)
{
	// calculate rotation properly!
	const Quat appliedRotation = Quat::identity;
	setRotation(appliedRotation);
}

void Transform::setScale(const float3 scale)
{
	_scale = scale;
}

//----
void Transform::setTransform(const float3 position, const Quat rotation, const float3 scale)
{
	setPosition(position);
	setRotation(rotation);
	setScale(scale);
}

float4x4 Transform::getTransformMatrix() const
{
	return float4x4::FromTRS(_position, _rotation, _scale);
}


void Transform::recacheTransformIfNeeded()
{
	if (_scale.Equals(_cachedScale) && _rotation.Equals(_cachedRotation) && _position.Equals(_cachedPosition))
	{
		return;
	}
	_cachedPosition = _position;
	_cachedRotation = _rotation;
	_cachedScale = _scale;
}

// ----
void Transform::OnEditor()
{
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		float pos[3] = { _position.x, _position.y, _position.z };
		float scl[3] = { _scale.x, _scale.y, _scale.z };
		float3 tmprot = _rotation.ToEulerXYZ() * 180.0f / pi;
		float rot[3] = { tmprot.x, tmprot.y, tmprot.z };

		ImGui::DragFloat3("Position", pos, 0.1f);
		ImGui::DragFloat3("Rotation", rot, 2.0f);
		ImGui::DragFloat3("Scale", scl, 0.1f);

		_position = { pos[0], pos[1], pos[2] };
		_scale = { scl[0], scl[1], scl[2] };
		_rotation = Quat::FromEulerXYZ(rot[0] * pi / 180.0f, rot[1] * pi / 180.0f, rot[2] * pi / 180.0f);
	}
}