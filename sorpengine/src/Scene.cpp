#include "Scene.hpp"

Scene::Scene(SceneManager& sceneManager) : _sceneManager(sceneManager)
{}

bool Scene::init()
{
	return true;
}

bool Scene::start()
{
	return true;
}

UpdateStatus Scene::preUpdate()
{
	return UpdateStatus::Continue;
}

UpdateStatus Scene::update(float)
{
	return UpdateStatus::Continue;
}

UpdateStatus Scene::postUpdate()
{
	return UpdateStatus::Continue;
}

bool Scene::cleanUp()
{
	return true;
}
