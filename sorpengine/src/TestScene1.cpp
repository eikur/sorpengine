#include "TestScene1.hpp"

#include "Application.hpp"
#include "ModuleInput.hpp"
#include "ModuleWindow.hpp"
#include "TextureHelper.hpp"
#include "ShaderManager.hpp"
#include "Utils.hpp"
#include "Camera.hpp"

#include "GameObject\GameObject.hpp"

namespace
{
	const std::string kTextureName = "resources/Lenna.png";
}

TestScene1::TestScene1(SceneManager& sceneManager) : Scene(sceneManager)
{}

bool TestScene1::init()
{
	addGameObject(new GameObject("test"));
	return Scene::init();
}

UpdateStatus TestScene1::update(float dt)
{
	return Scene::update(dt);
}
