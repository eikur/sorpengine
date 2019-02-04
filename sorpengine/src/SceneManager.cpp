#include "SceneManager.hpp"

#include "TestScene1.hpp"
#include "GameObject\GameObject.hpp"
#include "GameObject/ComponentFactory.hpp"

SceneManager::SceneManager(const Application& application, const ModuleInput& input, bool active) : Module(active), _application(application), _input(input)
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
    handleEditorCamera();

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

void SceneManager::swapScene(SceneId sceneId, float /*duration*/)
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

GameObject& SceneManager::getCurrentSceneRoot() const
{
	return _currentScene->getSceneRoot();
}


//-- GameObject related stuff
GameObject* SceneManager::addNewGameObject(GameObject* parent)
{
    const std::string kDefaultGameObjectName = "GameObject";
    std::unique_ptr<GameObject> go = std::make_unique<GameObject>(kDefaultGameObjectName);
    go->addComponent(ComponentFactory().createComponent<Transform>());

    GameObject* retVal = go.get();

    if (parent == nullptr)
    {
        getCurrentSceneRoot().addChild(std::move(go));
    }
    else
    {
        parent->addChild(std::move(go));
    }

    return retVal;
}

void SceneManager::addComponentToGameObject(GameObject* target, ComponentType type)
{
    if (target == nullptr)
    {
        return;
    }
    
    if (type == ComponentType::Transform || type == ComponentType::Script)
    {
        return;
    }
    
    std::unique_ptr<Component> componentToAdd;

    switch (type)
    {
        case ComponentType::Camera:
            componentToAdd = ComponentFactory().createComponent<Camera>();
            break;
        case ComponentType::Image:
            componentToAdd = ComponentFactory().createComponent<Image>();
            break;
        case ComponentType::Material:
            componentToAdd = ComponentFactory().createComponent<MaterialComponent>();
            break;
        case ComponentType::Mesh:
            componentToAdd = ComponentFactory().createComponent<MeshComponent>();
            break;
        default:
            break;
    }
    
    if (componentToAdd)
    {
        componentToAdd->init();
        target->addComponent(std::move(componentToAdd));
    }
}

void SceneManager::initEditorCamera(const float aspectRatio)
{
    _editorCamera.Init(aspectRatio);
}

void SceneManager::translateEditorCamera(const float3& translation)
{
    _editorCamera.translate(translation);
}

void SceneManager::showEditorCameraProperties()
{
    _editorCamera.onEditor();
}

const Camera& SceneManager::getCurrentSceneCamera() const
{
    if (_application.isInGameMode() && _currentScene)
    {
        const Camera* activeCamera = _currentScene->getActiveCamera();
        return activeCamera != nullptr ? *activeCamera : _editorCamera;
    }

    return _editorCamera;
}

void SceneManager::handleEditorCamera()
{
    if (_input.getKey(SDL_SCANCODE_C) == ModuleInput::KeyState::Down)
    {
        _editorCamera.switchType();
    }

    if (_input.getKey(SDL_SCANCODE_W) == ModuleInput::KeyState::Repeat)
    {
        translateEditorCamera(_editorCamera.front() * _editorCameraSpeed * _editorCameraDelta);
    }
    else if (_input.getKey(SDL_SCANCODE_S) == ModuleInput::KeyState::Repeat)
    {
        translateEditorCamera(-_editorCamera.front() * _editorCameraSpeed * _editorCameraDelta);
    }

    if (_input.getKey(SDL_SCANCODE_Q) == ModuleInput::KeyState::Repeat)
    {
        translateEditorCamera(-_editorCamera.up() * _editorCameraSpeed * _editorCameraDelta);
    }
    else if (_input.getKey(SDL_SCANCODE_E) == ModuleInput::KeyState::Repeat)
    {
        translateEditorCamera(_editorCamera.up() * _editorCameraSpeed * _editorCameraDelta);
    }

    if (_input.getKey(SDL_SCANCODE_A) == ModuleInput::KeyState::Repeat)
    {
        translateEditorCamera(-_editorCamera.right() * _editorCameraSpeed * _editorCameraDelta);
    }
    else if (_input.getKey(SDL_SCANCODE_D) == ModuleInput::KeyState::Repeat)
    {
        translateEditorCamera(_editorCamera.right() * _editorCameraSpeed * _editorCameraDelta);
    }
}