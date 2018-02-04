#include "SceneManager.hpp"
#include "Application.hpp"
#include "ModuleInput.hpp"
#include "ModuleWindow.hpp"
#include "ShaderManager.hpp"
#include "Utils.hpp"
#include "Camera.hpp"

SceneManager::SceneManager(bool active) : Module(active) {}

bool SceneManager::init()
{
	return true;
}

UpdateStatus SceneManager::preUpdate()
{
	return UpdateStatus::Continue;
}

UpdateStatus SceneManager::update(float)
{
	auto& inputModule = App->getInput();	 
	if (inputModule.getKey(SDL_SCANCODE_ESCAPE))
	{
		return UpdateStatus::Stop;
	}
	float z = 0.8f;
	float w = 1.0f;

	glBegin(GL_POLYGON);
	glVertex3f(w/2, w/2, -z);
	glVertex3f(w/2, -w/2, -z);
	glVertex3f(-w/2, -w/2, -z);
	glVertex3f(-w/2, w/2, -z);
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

UpdateStatus SceneManager::postUpdate()
{
	return UpdateStatus::Continue;
}

bool SceneManager::cleanUp()
{
	return true;
}