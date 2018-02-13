#include "TextureHelper.hpp"

#include "Utils.hpp"
#include "SDL.h"
#include "SDL_image.h"


bool TextureHelper::init()
{
	return true;
}

bool TextureHelper::cleanUp()
{
	_textures.clear();
	return true;
}

int TextureHelper::loadTexture(const std::string& texture_path)
{
	return 0;
}

int TextureHelper::createCheckersTexture()
{
	return 0;
}

void TextureHelper::useTexture(GLuint textureId)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
}
