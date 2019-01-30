#include "ModuleGUI.hpp"

#include "Application.hpp"
#include "GameObject/GameObject.hpp"
#include "ModuleWindow.hpp"
#include <Windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"
#include "ImGui/imgui_internal.h"

ModuleGUI::ModuleGUI(SceneManager& sceneManager, ModuleWindow& moduleWindow, bool active) 
    : Module(active)
    , _sceneManager(sceneManager)
    , _moduleWindow(moduleWindow)
{}

bool ModuleGUI::init()
{
	ImGui_ImplSdlGL3_Init(App->getWindow().getSDLWindow());
	initStyle();

    // default open windows: hierarchy and inspector
    _data.showHierarchy = true;
    _data.showInspector = true;
	return true;
}

UpdateStatus ModuleGUI::update(float)
{
	ImVec4 clear_color = ImColor(0, 0, 0);

	ImGui_ImplSdlGL3_NewFrame(App->getWindow().getSDLWindow());

	if (_data.showAbout) 
	{ 
		showAbout(&_data.showAbout);
	}

	//static bool test = true;
	//ImGui::ShowTestWindow(&test);
	if (_data.showHierarchy)
	{
		showHierarchy();
	}

	if (_data.showInspector)
	{
		showInspector();
	}

    if (_data.showEditorCameraProperties)
    {
        showEditorCameraProperties();
    }

	if (showMainMenu())
	{
		draw();
		return UpdateStatus::Continue;
	}

	return UpdateStatus::Stop;
}

bool ModuleGUI::cleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleGUI::draw()
{
	ImGui::Render();
}

bool ModuleGUI::showMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Hierarchy", nullptr, &_data.showHierarchy)) {};
			if (ImGui::MenuItem("Inspector", nullptr, &_data.showInspector)) {};
            if (ImGui::MenuItem("Editor Camera", nullptr, &_data.showEditorCameraProperties)) {};
			ImGui::EndMenu();
		}
        if (ImGui::BeginMenu("GameObject"))
        {
            if (ImGui::MenuItem("Add new", nullptr, nullptr)) { addNewGameObjectToScene(); };
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Component"))
        {
            if (ImGui::MenuItem("Add Image", nullptr, nullptr)) { addComponentToSelectedGameObject(ComponentType::Image); };
            if (ImGui::MenuItem("Add Camera", nullptr, nullptr)) { addComponentToSelectedGameObject(ComponentType::Camera); };
            ImGui::EndMenu();
        }

		if (ImGui::BeginMenu("Documentation"))
		{
			if (ImGui::MenuItem("About", nullptr, &_data.showAbout)) {}
			ImGui::EndMenu();
		}
        ImGui::EndMainMenuBar();
	}
	return true;
}

void ModuleGUI::showHierarchy()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::SetNextWindowSize(ImVec2(260, 320), ImGuiSetCond_Appearing);
	if (!ImGui::Begin("Hierarchy", &(bool)_data.showHierarchy, window_flags))
	{
		ImGui::End();
		return;
	}

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	int index = 0;
	GameObject* sceneRoot = _sceneManager.getCurrentSceneRoot();
	sceneRoot->onHierarchy(index, nodeFlags, _data.selectedGameObject);

	ImGui::End();
}

void ModuleGUI::showInspector()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(280, 500), ImGuiSetCond_Appearing);
	if (!ImGui::Begin("Inspector", &(bool)_data.showInspector, window_flags))
	{
		ImGui::End();
		return;
	}

	if (!_data.selectedGameObject)
	{
		ImGui::End();
		return;
	}

	_data.selectedGameObject->onEditor();
	ImGui::End();
}

void ModuleGUI::showAbout(bool* enabled) const
{
	ImGui::SetNextWindowPos(ImVec2(150.0f, 100.0f));
	if (ImGui::Begin("About", enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize) == false) {
		ImGui::End();
		return;
	}
	ImGui::Text("SorpEngine");
	ImGui::Text("Jorge Soriano");
	ImGui::End();
}

void ModuleGUI::initStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(15, 15);
	style.WindowRounding = 5.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

void ModuleGUI::addNewGameObjectToScene()
{
    _data.selectedGameObject =  _sceneManager.addNewGameObject();
}

void ModuleGUI::addComponentToSelectedGameObject(ComponentType type)
{
    _sceneManager.addComponentToGameObject(_data.selectedGameObject, type);
}

void ModuleGUI::showEditorCameraProperties()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::SetNextWindowSize(ImVec2(260, 320), ImGuiSetCond_Appearing);
    if (!ImGui::Begin("Editor Camera", &(bool)_data.showEditorCameraProperties, window_flags))
    {
        ImGui::End();
        return;
    }
    
    _moduleWindow.showCameraProperties();

    ImGui::End();
}