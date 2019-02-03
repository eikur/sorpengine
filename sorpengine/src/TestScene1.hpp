#pragma once

#include "Scene.hpp"

class GameObject;

class TestScene1 : public Scene
{
public:
	TestScene1(SceneManager& manager);

	bool init() override;
	UpdateStatus update(float dt = 0.0f) override;
};