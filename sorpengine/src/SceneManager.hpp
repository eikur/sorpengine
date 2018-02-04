#pragma once

#include "Module.hpp"
#include "Macros.hpp"
#include "Globals.hpp"

class Scene;

class SceneManager : public Module
{
public:
	enum class SceneId
	{
		SceneTest
	};

public:
	SceneManager(bool active = true);

	bool init() override;
	UpdateStatus preUpdate() override;
	UpdateStatus update(float dt = 0.0f) override;
	UpdateStatus postUpdate() override;
	bool cleanUp() override;

	void swapScene(SceneId sceneId, float duration = 0.0f);

private:
	bool isInTransition() const;
	void updateTransition(float dt);

private:
	Scene* _currentScene = nullptr;
	Scene* _nextScene = nullptr;

	MEMBER_DECL(Scene, _sceneTest, getSceneTest)

	bool _inTransition = false;
	float _transitionTime = 0.0f;
	float _transitionDuration = 0.0f;

};
