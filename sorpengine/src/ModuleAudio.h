#pragma once
#include "Module.h"
#include <vector>

struct _Mix_Music;
struct Mix_Chunk;

class ModuleAudio : public Module
{
	static float kFadeTimeDefault;
	using MixMusic = _Mix_Music;
	using MixChunk = Mix_Chunk;

public:
	ModuleAudio(bool active = true);
	virtual ~ModuleAudio() {}

	bool init() override;
	bool cleanUp() override;

	bool PlayMusic(const std::string& path, float fadeTime = kFadeTimeDefault) const;

	int LoadFx(const std::string& path);
	bool PlayFx(int fx, int repeatTimes = 0) const;

private:
	MixMusic* music = nullptr;
	std::vector<MixChunk*> fx;
};
