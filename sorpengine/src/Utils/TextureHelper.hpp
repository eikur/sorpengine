#pragma once

#include <map>
#include <string>
#include "MathGeoLib.h"

class TextureHelper
{
	using MTextureNameToId = std::map<std::string, unsigned>;
	using PTextureNameToId = std::pair<std::string, unsigned>;

public: 
	int kInvalidTextureId = 0;

public:
	void init();
	void finalize();

	int loadTexture(const std::string& texturePath);
	void useTexture(int textureId);
	void stopUsingTexture();
	void unloadTexture(int textureId);

private:
	MTextureNameToId _textures;
};