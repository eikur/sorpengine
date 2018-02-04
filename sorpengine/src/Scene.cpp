#include "Scene.hpp"

#include "Application.hpp"
#include "ModuleInput.hpp"
#include "ModuleWindow.hpp"
#include "ShaderManager.hpp"
#include "Utils.hpp"
#include "Camera.hpp"

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
	auto& inputModule = App->getInput();
	if (inputModule.getKey(SDL_SCANCODE_ESCAPE))
	{
		return UpdateStatus::Stop;
	}
	float z = 0.8f;
	float w = 1.0f;

	glBegin(GL_POLYGON);
	glVertex3f(w / 2, w / 2, -z);
	glVertex3f(w / 2, -w / 2, -z);
	glVertex3f(-w / 2, -w / 2, -z);
	glVertex3f(-w / 2, w / 2, -z);
	glEnd();

	//	App->getShaderManager().UseProgram("test1");

	Camera& cam = App->getWindow().getCamera();
	float3 pos = cam.GetPosition();
	if (inputModule.getKey(SDL_SCANCODE_UP))
	{
		pos -= float3::unitZ * 0.02f;
		cam.SetPosition(pos);
	}
	else if (inputModule.getKey(SDL_SCANCODE_DOWN))
	{
		pos += float3::unitZ * 0.02f;
		cam.SetPosition(pos);
	}
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
