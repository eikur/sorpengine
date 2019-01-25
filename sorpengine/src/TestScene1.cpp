#include "TestScene1.hpp"

#include "Application.hpp"
#include "ModelHelper.hpp"
#include "ModuleInput.hpp"
#include "ModuleWindow.hpp"
#include "TextureHelper.hpp"
#include "ShaderManager.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "GameObject\Image.hpp"

#include "GameObject\GameObject.hpp"
#include "GameObject/ComponentFactory.hpp"

TestScene1::TestScene1(SceneManager& sceneManager) : Scene(sceneManager)
{}

bool TestScene1::init()
{
	GameObject* go = new GameObject("test");
    go->addTransform(ComponentFactory().createComponent<Transform>());
	go->addComponent(ComponentFactory().createComponent<Image>());
	addGameObject(go);

    const std::string kModelName = "resources/magnetto2.fbx";
    addGameObject(App->getModelHelper().getGameObjectFromModel(kModelName));

	return Scene::init();
}

UpdateStatus TestScene1::update(float dt)
{
	return Scene::update(dt);
}
