#include "Application.hpp"
#include "Utils.hpp"

#include "MemLeaks.h"
#include <stdlib.h>

#include "SDL.h"
#include "MathGeoLib.h"


#if _WIN64
#	define ENV64
#else
#	define ENV32
#endif

#ifdef _MSC_VER
#   ifdef ENV32
#		pragma comment(lib, "3rdparty/DevIL/libx86/rel/ILU.lib")
#		pragma comment(lib, "3rdparty/DevIL/libx86/rel/ILUT.lib")
#		pragma comment(lib, "3rdparty/DevIL/libx86/rel/DevIL.lib")
#		pragma comment(lib, "3rdparty/SDL/libx86/SDL2.lib")
#		pragma comment(lib, "3rdparty/SDL/libx86/SDL2main.lib")
#		pragma comment(lib, "3rdparty/SDL_mixer/libx86/SDL2_mixer.lib")
#		ifdef _DEBUG
#			pragma comment( lib, "3rdparty/glew-2.0.0/libx86/deb/glew32d.lib" )
#			pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#			pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#			pragma comment( lib, "3rdparty/MathGeoLib/libx86/deb/MathGeoLib.lib" )
#           pragma comment (lib, "3rdparty/assimp/lib/assimp-vc140-mt.lib")
#		else // RELEASE
#			pragma comment( lib, "3rdparty/glew-2.0.0/libx86/rel/glew32.lib" )
#			pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#			pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#			pragma comment( lib, "3rdparty/MathGeoLib/libx86/rel/MathGeoLib.lib" )
#           pragma comment (lib, "3rdparty/assimp/lib/assimp-vc140-mt.lib")
#		endif
#   else 
#		pragma comment(lib, "3rdparty/DevIL/libx64/rel/ILU.lib")
#		pragma comment(lib, "3rdparty/DevIL/libx64/rel/ILUT.lib")
#		pragma comment(lib, "3rdparty/DevIL/libx64/rel/DevIL.lib")
#		pragma comment(lib, "3rdparty/SDL/libx64/SDL2.lib")
#		pragma comment(lib, "3rdparty/SDL/libx64/SDL2main.lib")
#		pragma comment(lib, "3rdparty/SDL_mixer/libx64/SDL2_mixer.lib")
#		ifdef _DEBUG
#			pragma comment( lib, "3rdparty/glew-2.0.0/libx64/deb/glew32d.lib" )
#			pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#			pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#			pragma comment( lib, "3rdparty/MathGeoLib/libx64/deb/MathGeoLib.lib" )
#           pragma comment (lib, "3rdparty/assimp/lib/assimp-vc140-mt.lib")
#		else // RELEASE
#			pragma comment( lib, "3rdparty/glew-2.0.0/libx86/rel/glew32.lib" )
#			pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#			pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#			pragma comment( lib, "3rdparty/MathGeoLib/libx86/rel/MathGeoLib.lib" )
#           pragma comment (lib, "3rdparty/assimp/lib/assimp-vc140-mt.lib")
#		endif
#	endif
#endif

enum class MainState
{
	Creation,
	Start,
	Update,
	Finish,
	Exit
};

Application* App = nullptr;

int main(int argc, char *argv[])
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