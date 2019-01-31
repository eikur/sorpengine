#pragma once

#include "Macros.hpp"
#include "Module.hpp"
#include "Globals.hpp"

#define SCENE_DECL(_CLASS_, _NAME_, _GETTER_)				\
		private:											\
			std::shared_ptr<_CLASS_> _NAME_;				\
		public:												\
			Scene& _GETTER_() const							\
			{												\
				return *(Scene*)_NAME_.get();	\
			}

class GameObject;
class Scene;
class TestScene1;

enum class ComponentType;

class SceneManager : public Module
{
public:
	enum class SceneId
	{
        SceneTest,
		TestScene1
	};

public:
	SceneManager(bool active = true);
	~SceneManager() override;

	bool init() override;
	bool start() override;
	UpdateStatus preUpdate() override;
	UpdateStatus update(float dt = 0.0f) override;
	UpdateStatus postUpdate() override;
	bool cleanUp() override;

	void swapScene(SceneId sceneId, float duration = 0.0f);

	std::string getCurrentSceneName() const;
	GameObject* getCurrentSceneRoot() const;

    GameObject* addNewGameObject(GameObject* parent = nullptr);
    void addComponentToGameObject(GameObject* target, ComponentType type);


private:
	bool isInTransition() const;
	void updateTransition(float dt);

private:
	Scene* _currentScene = nullptr;
	Scene* _nextScene = nullptr;

	SCENE_DECL(TestScene1, _testScene1, getTestScene1)


	bool _inTransition = false;
	float _transitionTime = 0.0f;
	float _transitionDuration = 0.0f;

};
