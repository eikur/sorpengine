#include "ModuleGUI.hpp"

#include "Application.hpp"
#include "ModuleWindow.hpp"
#include <Windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl_gl3.h"
#include "ImGui/imgui_internal.h"

ModuleGUI::ModuleGUI(bool active) : Module(active)
{}

bool ModuleGUI::init()
{
	ImGui_ImplSdlGL3_Init(App->getWindow().getSDLWindow());
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
		if (ImGui::BeginMenu("Documentation"))
		{
			if (ImGui::MenuItem("About", NULL, &_data.showAbout)) {}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	return true;
}

void ModuleGUI::showAbout(bool* enabled) const
{
	ImGui::SetNextWindowPos(ImVec2(150.0f, 100.0f));
	if (ImGui::Begin("About", enabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize) == false) {
		ImGui::End();
		return;
	}
	ImGui::Separator();
	ImGui::Text("SorpEngine");
	ImGui::Text("Jorge Soriano");
	ImGui::Separator();
	ImGui::End();
}