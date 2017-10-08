#include "ModuleScene.h"
#include "Application.h"
#include "ModuleInput.h"

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