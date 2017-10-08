#include "ModuleScene.h"

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