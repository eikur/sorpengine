#include "SceneManager.hpp"
#include "Scene.hpp"

SceneManager::SceneManager(bool active) : Module(active) 
{
	_sceneTest = std::make_unique<Scene>(*this);
}

bool SceneManager::init()
{
	getSceneTest().start();
	swapScene(SceneId::SceneTest);
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
		_nextScene = &getSceneTest();
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