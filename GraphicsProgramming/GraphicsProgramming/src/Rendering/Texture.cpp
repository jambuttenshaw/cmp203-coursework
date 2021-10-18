#include "Texture.h"

#include "SOIL.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <cassert>

Texture::Texture(std::string filepath, bool genMipmaps)
{
	int flags = SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT;
	if (genMipmaps)
	{
		flags |= SOIL_FLAG_MIPMAPS;
		hasMipmaps = true;
	}

	textureHandle = SOIL_load_OGL_texture(
		filepath.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		flags
	);

	assert(textureHandle && "Failed to load texture!");
}

Texture::~Texture()
{
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
	assert(mag <= FilterMode::Linear && "Invalid magnification filter!");
	magFilter = mag;

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
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLSampleMode(sampleModeX));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLSampleMode(sampleModeY));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLFilterMode(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetGLFilterMode(magFilter));

	Unbind();
}

void Texture::EnableTextures()
{
	glEnable(GL_TEXTURE_2D);
}

void Texture::DisableTextures()
{
	glDisable(GL_TEXTURE_2D);
}
