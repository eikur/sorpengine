/* Application.h */

#pragma once
#include <memory>
#include<vector>
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
	 std::vector<std::unique_ptr<Module>> modules;
};

extern Application* App;