#pragma once

#include "Globals.hpp"
#include "Macros.hpp"
#include "SceneManager.hpp"

class Scene
{
public:
	Scene(SceneManager& manager);

	virtual bool init();
	virtual bool start();
	virtual UpdateStatus preUpdate();
	virtual UpdateStatus update(float dt = 0.0f);
	virtual UpdateStatus postUpdate();
	virtual bool cleanUp();

private:
	MEMBER_REF_GET(SceneManager, _sceneManager, getSceneManager)
};