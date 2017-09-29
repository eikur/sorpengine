#pragma once

#include<list>
#include "Globals.h"

class Module;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

private:
	std::list<Module*> modules;
};

extern Application* App;