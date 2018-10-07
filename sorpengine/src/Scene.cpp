#include "Scene.hpp"

namespace
{
	const std::string kRootNodeName = "root";
}

Scene::Scene(SceneManager& sceneManager): _sceneManager(sceneManager), _sceneRoot(new GameObject(kRootNodeName))
{}

Scene::~Scene()
{
	delete _sceneRoot;
}

bool Scene::init()
{
	// load scene in children before init from base class
	return _sceneRoot->init();
}

bool Scene::start()
{
	return _sceneRoot->start();
}

UpdateStatus Scene::preUpdate()
{
	return _sceneRoot->preUpdate();
}

UpdateStatus Scene::update(float dt)
{
	return _sceneRoot->update(dt);
}

UpdateStatus Scene::postUpdate()
{
	return _sceneRoot->postUpdate();
}

bool Scene::cleanUp()
{
	return _sceneRoot->cleanUp();
}

void Scene::addGameObject(GameObject* gameObject)
{
	return _sceneRoot->addChild(gameObject);
}

void Scene::removeGameObject(GameObject* gameObject)
{
	return _sceneRoot->removeChild(gameObject);
}