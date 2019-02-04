#include "Scene.hpp"

namespace
{
	const std::string kRootNodeName = "root";
}

Scene::Scene(SceneManager& sceneManager): _sceneManager(sceneManager), _sceneRoot(std::make_unique<GameObject>(kRootNodeName))
{}

Scene::~Scene()
{
    _sceneRoot.reset();
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

void Scene::addGameObject(std::unique_ptr<GameObject>&& gameObject)
{
    return _sceneRoot->addChild(std::move(gameObject));
}

void Scene::removeGameObject(GameObject* gameObject)
{
	return _sceneRoot->removeChild(gameObject);
}

GameObject& Scene::getSceneRoot() const
{
    return *_sceneRoot.get();
}

const Camera* Scene::getActiveCamera() const
{
    const GameObject* cameraGo = _sceneRoot->findFirstChildWithCameraComponent();

    return cameraGo? cameraGo->getCameraComponent() : nullptr;
}