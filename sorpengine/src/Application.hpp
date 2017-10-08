/* Application.h */

#pragma once
#include <memory>
#include <vector>
#include "Globals.hpp"
#include "Module.hpp"

class Application
{
public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

	template <class T>
	T& getModule() const;

private:
	Module* findModule(Module::Type type) const;

	std::vector<std::unique_ptr<Module>> _modules;
};

extern Application* App;