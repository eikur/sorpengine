#pragma once

#include "Component.hpp"

class Material;

class MaterialComponent : public Component
{
public:
	MaterialComponent(Material* material);

	void onEditor() override;

private:
	Material * _material = nullptr;
};
