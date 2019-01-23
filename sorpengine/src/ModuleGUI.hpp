#pragma once

#include "Module.hpp"
#include "SceneManager.hpp"

class ModuleGUI : public Module
{
	struct Data
	{
		GameObject* selectedGameObject = nullptr;
		bool showInspector = false;

		bool showHierarchy = false;
		bool showAbout = false;
	};


public: 
	ModuleGUI(SceneManager& sceneManager, bool active = true);

	bool init() override;
	UpdateStatus update(float dt) override;
	bool cleanUp() override;

private:
	void initStyle();

	void draw();
	bool showMainMenu();
	void showHierarchy();
	void showInspector();
	void showAbout(bool *enabled) const;

private:
	Data _data;
	SceneManager& _sceneManager;

};