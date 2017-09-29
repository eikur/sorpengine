#include "Application.h"
#include "Utils.h"
#include <stdlib.h>

enum class MainState
{
	Creation,
	Start,
	Update,
	Finish,
	Exit
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	ReportMemoryLeaks();

	int main_return = EXIT_FAILURE;
	MainState mainState = MainState::Creation;

	while (mainState != MainState::Exit)
	{
		switch (mainState)
		{
		case MainState::Creation:

			Utils::log("Application Creation --------------");
			App = new Application();
			mainState = MainState::Start;
			break;

		case MainState::Start:

			Utils::log("Application Init --------------");
			if (App->Init() == false)
			{
				Utils::log("Application Init exits with error -----");
				mainState = MainState::Exit;
			}
			else
			{
				mainState = MainState::Update;
				Utils::log("Application Update --------------");
			}

			break;

		case MainState::Update:
		{
			UpdateStatus updateRet = App->Update();

			if (updateRet == UpdateStatus::Error)
			{
				Utils::log("Application Update exits with error -----");
				mainState = MainState::Exit;
			}

			if (updateRet == UpdateStatus::Stop)
				mainState = MainState::Finish;
		}
		break;

		case MainState::Finish:

			Utils::log("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				Utils::log("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			mainState = MainState::Exit;

			break;

		}
	}

	delete App;
	Utils::log("Exiting.\n");
	return main_return;
}