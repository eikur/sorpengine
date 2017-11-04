#include "ModuleScene.hpp"
#include "Application.hpp"
#include "ModuleInput.hpp"
#include "ModuleWindow.hpp"
#include "ShaderManager.hpp"
#include "Utils.hpp"
#include "Camera.hpp"

ModuleScene::ModuleScene(bool active) : Module(Module::Type::Scene, active) {}

bool ModuleScene::init()
{
	return true;
}

UpdateStatus ModuleScene::preUpdate()
{
	return UpdateStatus::Continue;
}

UpdateStatus ModuleScene::update(float)
{
	auto& inputModule = App->getModule<ModuleInput>();	 
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

	Camera& cam = App->getModule<ModuleWindow>().getCamera();
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

UpdateStatus ModuleScene::postUpdate()
{
	return UpdateStatus::Continue;
}

bool ModuleScene::cleanUp()
{
	return true;
}