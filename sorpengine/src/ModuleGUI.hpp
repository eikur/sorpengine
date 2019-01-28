#pragma once

#include "Module.hpp"
#include "SceneManager.hpp"
#include "ModuleWindow.hpp"

class ModuleGUI : public Module
{
	struct Data
	{
		GameObject* selectedGameObject = nullptr;
		bool showInspector = false;

		bool showHierarchy = false;
		bool showAbout = false;

        bool showEditorCameraProperties = false;
	};


public: 
	ModuleGUI(SceneManager& sceneManager, ModuleWindow& moduleWindow, bool active = true);

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

    void addNewGameObjectToScene();
    void addComponentToSelectedGameObject(ComponentType type);

    void showEditorCameraProperties();

private:
	Data _data;
	SceneManager& _sceneManager;
    ModuleWindow& _moduleWindow;

};