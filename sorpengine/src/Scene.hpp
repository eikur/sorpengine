#pragma once

#include "Globals.hpp"
#include "Macros.hpp"
#include "SceneManager.hpp"
#include "GameObject\GameObject.hpp"

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

	void addGameObject(GameObject* gameObject);
	void removeGameObject(GameObject* gameObject);

private:
	MEMBER_REF_GET(SceneManager, _sceneManager, getSceneManager)
	GameObject* _sceneRoot = nullptr;
};