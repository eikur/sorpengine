#pragma once

#include "GL/glew.h"
#include <map>
#include <string>

class TextureHelper
{
public:
	bool init();
	bool cleanUp();
	int loadTexture(const std::string& textureName);
	int createCheckersTexture();
	void useTexture(GLuint textureId);

private:
	std::map<std::string, int> _textures;
};