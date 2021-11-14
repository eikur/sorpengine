#pragma once

#include "Application.hpp"
#include "Module.hpp"
#include "SceneManager.hpp"
#include "ModuleWindow.hpp"
#include "TimeManager.hpp"

class ModuleGUI : public Module
{
	struct Data
	{
		GameObject* selectedGameObject = nullptr;
		bool showInspector = false;

		bool showHierarchy = false;
		bool showAbout = false;

        bool showEditorCameraProperties = false;

        // always enabled, ImGui doesn't allow for const bool though
        bool showEditorPlaybackControls = true;
	};


public: 
	ModuleGUI(Application& application, SceneManager& sceneManager, ModuleWindow& moduleWindow);

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

    void addNewGameObjectToScene(GameObject* parent = nullptr);
    void addComponentToSelectedGameObject(ComponentType type);

    void showEditorCameraProperties();
    void showEditorPlaybackControls();

private:
	Data _data;
    Application& _application;
	SceneManager& _sceneManager;
    ModuleWindow& _moduleWindow;
};