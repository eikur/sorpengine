#include "ModuleScene.hpp"
#include "Application.hpp"
#include "ModuleInput.hpp"
#include "ShaderManager.hpp"

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
	float z = 0;
	float w = 1.0f;

	glBegin(GL_POLYGON);
	glVertex3f(w, w, -z);
	glVertex3f(w, -w, -z);
	glVertex3f(-w, -w, -z);
	glVertex3f(-w, w, -z);
	glEnd();
		
	App->getShaderManager().UseProgram("test1");
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