#include "Transform.hpp"

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
	// remove me
	float w = 0.1f;

	App->getTextureHelper().useTexture(_textureId);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(_position.x - w / 2, _position.y - w / 2, _position.z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(_position.x + w / 2, _position.y - w / 2, _position.z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(_position.x + w / 2, _position.y + w / 2, _position.z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(_position.x - w / 2, _position.y + w / 2, _position.z);
	glEnd();

	return UpdateStatus::Continue;
}

// -- transform modification methods
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