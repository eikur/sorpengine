#pragma once
#include "Module.hpp"

class SceneManager : public Module
{
public:
	SceneManager(bool active = true);
	virtual ~SceneManager() {}

	bool init() override;
	UpdateStatus preUpdate() override;
	UpdateStatus update(float dt = 0.0f) override;
	UpdateStatus postUpdate() override;
	bool cleanUp() override;
};
