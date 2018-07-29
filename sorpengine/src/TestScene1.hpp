#pragma once

#include "Scene.hpp"

class GameObject;

class TestScene1 : public Scene
{
public:
	TestScene1(SceneManager& manager);
	~TestScene1() = default;

	bool init() override;
	UpdateStatus update(float dt = 0.0f) override;

  private:
	int _textureId = 0;
};