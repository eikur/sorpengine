#pragma once

#include "Globals.hpp"
#include "Macros.hpp"
#include "SceneManager.hpp"
#include "GameObject\GameObject.hpp"

class Scene
{
public:
	Scene(SceneManager& manager);
	virtual ~Scene();

	virtual bool init();
	virtual bool start();
	virtual UpdateStatus preUpdate();
	virtual UpdateStatus update(float dt = 0.0f);
	virtual UpdateStatus postUpdate();
	virtual bool cleanUp();

    void addGameObject(std::unique_ptr<GameObject>&& gameObject);
	void removeGameObject(GameObject* gameObject);

	GameObject& getSceneRoot() const;

private:
    MEMBER_REF_GET(SceneManager, _sceneManager, getSceneManager)
    std::unique_ptr<GameObject> _sceneRoot;
};