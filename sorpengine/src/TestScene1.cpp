#include "TestScene1.hpp"

#include "Application.hpp"
#include "ModuleInput.hpp"
#include "ModuleWindow.hpp"
#include "TextureHelper.hpp"
#include "ShaderManager.hpp"
#include "Utils.hpp"
#include "Camera.hpp"
#include "GameObject\Image.hpp"

#include "GameObject\GameObject.hpp"

namespace
{
	const std::string kTextureName = "resources/Lenna.png";
}

TestScene1::TestScene1(SceneManager& sceneManager) : Scene(sceneManager)
{}

bool TestScene1::init()
{
	GameObject* go = new GameObject("test");
	auto img = std::make_shared<Image>();
	go->addComponent(std::move(img));
	addGameObject(go);

	return Scene::init();
}

UpdateStatus TestScene1::update(float dt)
{
	return Scene::update(dt);
}
