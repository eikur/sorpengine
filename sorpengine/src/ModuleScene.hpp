#pragma once
#include "Module.hpp"

class ModuleScene : public Module
{
public:
	ModuleScene(bool active = true);
	virtual ~ModuleScene() {}

	bool init() override;
	UpdateStatus preUpdate() override;
	UpdateStatus update(float dt = 0.0f) override;
	UpdateStatus postUpdate() override;
	bool cleanUp() override;
};
