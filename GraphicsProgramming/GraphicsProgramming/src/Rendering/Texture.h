#pragma once

#include <string>


class Texture
{
public:
	// some enums to represent properties of a texture

	enum Flags : char // we will need no more than 8 bits to construct our bitmask with all the flags for loading a texture
					  // also it needs to be an unsigned type
	{
		MIPMAPS = 1,
		INVERT_Y = 2
	};

	// sample modes
	enum class SampleMode
	{
		Repeat,
		Clamp
	};

	// filter modes
	enum class FilterMode
	{
		Point,
		Linear,
		PointMipMapPoint,
		PointMipMapLinear,
		LinearMipMapPoint,
		LinearMipMapLinear,
	};

public:
	// load a texture from a file path with given flags
	Texture(std::string filepath, char flags = 0);
	~Texture();

	// set properties of the texture
	inline void SetSampleMode(SampleMode s) { SetSampleMode(s, s); }
	void SetSampleMode(SampleMode x, SampleMode y); // allow different sample modes on each axis
	void SetFilterMode(FilterMode min, FilterMode mag);

	// bind texture
	void Bind();
	void Unbind();

private:

	// udpate the properties of the texture with opengl
	int GetGLSampleMode(SampleMode mode);
	int GetGLFilterMode(FilterMode mode);

	void UpdateParameters();

private:
	// the opengl handle of the texture
	unsigned int textureHandle = 0;

	SampleMode sampleModeX = SampleMode::Clamp;
	SampleMode sampleModeY = SampleMode::Clamp;

	FilterMode minFilter = FilterMode::Point;
	FilterMode magFilter = FilterMode::Point;

	bool hasMipmaps = false;
};
