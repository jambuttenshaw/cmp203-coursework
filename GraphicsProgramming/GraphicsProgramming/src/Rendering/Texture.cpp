#include "Texture.h"

#include "SOIL.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <cassert>

Texture::Texture(std::string filepath, char flags)
{
	// load the texture using SOIL

	// first create a bitmask with the appropriate flags
	int soilFlags = SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT;
	if ((flags & MIPMAPS) != 0)
	{
		soilFlags |= SOIL_FLAG_MIPMAPS;
		hasMipmaps = true;
	}
	if ((flags & INVERT_Y) != 0)
	{
		soilFlags |= SOIL_FLAG_INVERT_Y;
	}
	// load texture
	textureHandle = SOIL_load_OGL_texture(
		filepath.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		soilFlags
	);
	// make sure it loaded correctly
	assert(textureHandle && "Failed to load texture!");
}

Texture::~Texture()
{
	// make sure to delete the texture!
	glDeleteTextures(1, &textureHandle);
}

void Texture::SetSampleMode(SampleMode x, SampleMode y)
{
	sampleModeX = x;
	sampleModeY = y;

	UpdateParameters();
}

void Texture::SetFilterMode(FilterMode min, FilterMode mag)
{
	// mag filter cannot include mipmaps
	assert(mag <= FilterMode::Linear && "Invalid magnification filter!");
	magFilter = mag;

	// dont use mipmap filter without generating mipmaps
	if (!hasMipmaps) assert(mag <= FilterMode::Linear && "This texture doesn't have mipmaps");
	minFilter = min;

	UpdateParameters();
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, textureHandle);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
}

// helper functions for getting the opengl enum value for each of our own enum values
int Texture::GetGLSampleMode(SampleMode mode)
{
	switch (mode)
	{
	case SampleMode::Clamp:		return GL_CLAMP;
	case SampleMode::Repeat:	return GL_REPEAT;
	}
	return GL_INVALID_ENUM;
}

int Texture::GetGLFilterMode(FilterMode mode)
{	
	switch (mode)
	{
	case FilterMode::Point:					return GL_NEAREST;
	case FilterMode::Linear:				return GL_LINEAR;
	case FilterMode::PointMipMapPoint:		return GL_NEAREST_MIPMAP_NEAREST;
	case FilterMode::PointMipMapLinear:		return GL_NEAREST_MIPMAP_LINEAR;
	case FilterMode::LinearMipMapPoint:		return GL_LINEAR_MIPMAP_NEAREST;
	case FilterMode::LinearMipMapLinear:	return GL_LINEAR_MIPMAP_LINEAR;
	}
	return GL_INVALID_ENUM;
}

void Texture::UpdateParameters()
{
	// update all the parameters of the texture
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLSampleMode(sampleModeX));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLSampleMode(sampleModeY));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterMode(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterMode(magFilter));

	Unbind();
}
