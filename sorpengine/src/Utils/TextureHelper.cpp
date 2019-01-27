#include "TextureHelper.hpp"

#include "glew-2.0.0\include\GL\glew.h"
#include "DevIL/include/IL/devil_cpp_wrapper.hpp"
#include "DevIL/include/IL/ilut.h"
#include "Utils.hpp"

void TextureHelper::init()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

void TextureHelper::finalize()
{
	for (const PTextureNameToId& kvp : _textures)
	{
		glDeleteTextures(1, &kvp.second);
	}
	_textures.clear();
}

int TextureHelper::loadTexture(const std::string& texturePath)
{
	const auto& it = _textures.find(texturePath);
	if (it != _textures.end())
	{
		return it->second;
	}

	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);
	if (ilLoadImage(texturePath.c_str()) == IL_FALSE)
	{
		Utils::log("ilLoadImage failed. Error: %s", iluErrorString(ilGetError()));
		ilDeleteImages(1, &imageId);
		return kInvalidTextureId;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	_textures[texturePath] = textureId;

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	ILinfo imageInfo;
	iluGetImageInfo(&imageInfo);
	if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	const int channels = ilGetInteger(IL_IMAGE_CHANNELS);
	if (channels == 3)
	{
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	}
	else if (channels == 4)
	{
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	}

    const GLenum target = GL_TEXTURE_2D;
    const int level = 0;
    const int internalFormat = ilGetInteger(IL_IMAGE_FORMAT);
	const int width = ilGetInteger(IL_IMAGE_WIDTH);
	const int height = ilGetInteger(IL_IMAGE_HEIGHT);
    const int border = 0; // must be 0
    const GLenum format = ilGetInteger(IL_IMAGE_FORMAT);
    const GLenum type = ilGetInteger(IL_IMAGE_TYPE);
    ILubyte* data = imageInfo.Data;

	glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
	ilDeleteImages(1, &imageId);

	return textureId;
}

void TextureHelper::useTexture(int textureId)
{
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void TextureHelper::unloadTexture(const int textureId)
{
	GLuint texId = static_cast<GLuint>(textureId);
	glDeleteTextures(1, &texId);
}

void TextureHelper::stopUsingTexture()
{
	useTexture(0);
}