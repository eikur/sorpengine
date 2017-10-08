#pragma once
#include "Module.h"
#include <vector>

struct _Mix_Music;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class ModuleAudio : public Module
{
	static float kFadeTimeDefault;

public:
	ModuleAudio(bool active = true);
	virtual ~ModuleAudio() {}

	bool init() override;
	bool cleanUp() override;

	bool PlayMusic(const std::string& path, float fadeTime = kFadeTimeDefault);

	int LoadFx(const std::string& path);
	bool PlayFx(int fx, int repeatTimes = 0) const;

private:
	Mix_Music* _music = nullptr;
	std::vector<Mix_Chunk*> _fx;
};
