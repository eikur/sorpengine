#include "SceneManager.hpp"
#include "TestScene1.hpp"
#include "GameObject\GameObject.hpp"

SceneManager::SceneManager(bool active) : Module(active) 
{
	_testScene1 = std::make_shared<TestScene1>(*this);
}

SceneManager::~SceneManager()
{
	_testScene1.reset();
}

bool SceneManager::init()
{
	getTestScene1().init();
	return true;
}

bool SceneManager::start()
{
	swapScene(SceneId::TestScene1);
	return true;
}

UpdateStatus SceneManager::preUpdate()
{
	if (_currentScene)
	{
		return _currentScene->preUpdate();
	}
	return UpdateStatus::Continue;
}

UpdateStatus SceneManager::update(float dt)
{
	if (_currentScene)
	{
		return _currentScene->update(dt);
	}
	else if (isInTransition())
	{
		updateTransition(dt);
		return UpdateStatus::Continue;
	}
	return UpdateStatus::Stop;
}

UpdateStatus SceneManager::postUpdate()
{
	if (_currentScene)
	{
		return _currentScene->postUpdate();
	}
	return UpdateStatus::Continue;
}

bool SceneManager::cleanUp()
{
	_testScene1->cleanUp();
	return true;
}

void SceneManager::swapScene(SceneId sceneId, float duration)
{
	if (_inTransition)
	{
		return;
	}

	switch (sceneId)
	{
	case SceneId::SceneTest:
	case SceneId::TestScene1:
		_nextScene = &getTestScene1();
		break;
	}
	// TODO: update Transitions
	/*_transitionDuration = duration * 0.5f;
	_transitionTime = _transitionDuration;
	_inTransition = true;*/
	_currentScene = _nextScene;
	_nextScene = nullptr;
}

bool SceneManager::isInTransition() const
{
	return _inTransition;
}

void SceneManager::updateTransition(float dt)
{
// TODO
}

std::string SceneManager::getCurrentSceneName() const
{
	return "SceneDummyName";
}

GameObject* SceneManager::getCurrentSceneRoot() const
{
	return _currentScene->getSceneRoot();
}
