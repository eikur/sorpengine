#include "ModuleAudio.hpp"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "Utils.hpp"

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

	if (_music != nullptr)
	{
		Mix_FreeMusic(_music);
	}

	for (auto it = _fx.begin(); it != _fx.end(); ++it)
	{
		Mix_FreeChunk(*it);
	}
	_fx.clear();
	
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool ModuleAudio::PlayMusic(const std::string& path, float fadeTime)
{
	if (_music)
	{
		if (fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(static_cast<int>(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}
		// this call blocks until fade out is completed. Search a workaround
		Mix_FreeMusic(_music);
	}

	_music = Mix_LoadMUS(path.c_str());

	if (_music == nullptr)
	{
		Utils::log("Couldn't load music track %s. Error was: %s\n", path.c_str(), Mix_GetError());
		return false;
	}

	int loops = -1; // always loop
	if (fadeTime > 0.0f)
	{
		if (Mix_FadeInMusic(_music, loops, fadeTime) < 0)
		{
			Utils::log("Couldn't fade in music track %s. Error was: %s\n", path.c_str(), Mix_GetError());
			return false;
		}
		else
		{
			if (Mix_PlayMusic(_music, loops) < 0)
			{
				Utils::log("Couldn't start music track %s. Error was: %s\n", path.c_str(), Mix_GetError());
				return false;
			}
		}
	}

	Utils::log("Successfully playing music track %s\n", path.c_str()); 
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
