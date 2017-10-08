#include "ModuleAudio.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "Utils.h"

float ModuleAudio::kFadeTimeDefault = 2.0f;

ModuleAudio::ModuleAudio(bool active) : Module(Module::Type::Audio, active) {}

bool ModuleAudio::init() 
{
	Utils::log("Initializing Audio Module\n");
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		Utils::log("SDL_InitSubSystems(SDL_INIT_AUDIO) error: %s\n", SDL_GetError());
		return false;
	}

	Utils::log("Loading support for OGG format\n");
	int flags = MIX_INIT_OGG;
	if (Mix_Init(flags) != flags)
	{
		Utils::log("Could not load OGG support. Mix_init error: %s\n", Mix_GetError());
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		Utils::log("Mix_OpenAudio failed. Error: %s\n", Mix_GetError());
		return false;
	}

	return true;
}

bool ModuleAudio::cleanUp()
{
	Utils::log("Cleaning up Audio Module\n");

	if (music != nullptr)
	{
		Mix_FreeMusic(music);
	}

	for (std::vector<MixChunk*>::iterator it = fx.begin(); it != fx.end(); ++it)
	{
		Mix_FreeChunk(*it);
	}
	fx.clear();
	
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool ModuleAudio::PlayMusic(const std::string&, float ) const
{
	return true;
}

int ModuleAudio::LoadFx(const std::string& )
{
	return 0;
}

bool ModuleAudio::PlayFx(int, int) const
{
	return true;
}
