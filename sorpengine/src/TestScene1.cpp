#include "TestScene1.hpp"

#include "Application.hpp"
#include "ModelHelper.hpp"
#include "ModuleInput.hpp"
#include "ModuleWindow.hpp"
#include "TextureHelper.hpp"
#include "ShaderManager.hpp"
#include "Utils.hpp"

#include "GameObject/ComponentFactory.hpp"
#include "GameObject/GameObject.hpp"

TestScene1::TestScene1(SceneManager& sceneManager) : Scene(sceneManager)
{}

bool TestScene1::init()
{
    // Consider creating a gameobject builder
    std::unique_ptr<GameObject> cameraGO = std::make_unique<GameObject>("MainCamera");
    cameraGO->addComponent(ComponentFactory().createComponent<Transform>(float3(0.f, 1.f, 5.f), Quat::identity, float3::one));
    cameraGO->addComponent(ComponentFactory().createComponent<Camera>());
    addGameObject(std::move(cameraGO));

    ModelHelper& modelHelper = App->getModelHelper();

    const std::string kModelName3 = "resources/ArmyPilot/ArmyPilot.dae";
    const std::string kAnimName = "resources/ArmyPilot/Animations/ArmyPilot_Run_Forwards.fbx";

    modelHelper.loadModel(kModelName3);
    modelHelper.loadAnimationsForModel(kModelName3, kAnimName);
    addGameObject(modelHelper.getGameObjectFromModel(kModelName3));

	return Scene::init();
}

UpdateStatus TestScene1::update(float dt)
{
	return Scene::update(dt);
}
