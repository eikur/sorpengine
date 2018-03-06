#pragma once

#include "Module.hpp"

class ModuleGUI : public Module
{
	struct Data
	{
		bool showAbout = false;
	};


public: 
	ModuleGUI(bool active = true);

	bool init() override;
	UpdateStatus update(float dt) override;
	bool cleanUp() override;

private:
	void initStyle();

	void draw();
	bool showMainMenu();
	void showAbout(bool *enabled) const;

private:
	Data _data;

};