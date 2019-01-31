#pragma once

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
	ModuleGUI(SceneManager& sceneManager, ModuleWindow& moduleWindow, TimeManager& timeManager, bool active = true);

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
	SceneManager& _sceneManager;
    ModuleWindow& _moduleWindow;
    TimeManager& _timeManager;
};