#pragma once

#include <string>


class Texture
{
public:
	enum Flags : char
	{
		MIPMAPS = 1,
		INVERT_Y = 2
	};

	enum class SampleMode
	{
		Repeat,
		Clamp
	};

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
	Texture(std::string filepath, char flags = 0);
	~Texture();

	inline void SetSampleMode(SampleMode s) { SetSampleMode(s, s); }
	void SetSampleMode(SampleMode x, SampleMode y);
	void SetFilterMode(FilterMode min, FilterMode mag);

	void Bind();
	void Unbind();

private:

	int GetGLSampleMode(SampleMode mode);
	int GetGLFilterMode(FilterMode mode);

	void UpdateParameters();

private:
	unsigned int textureHandle = 0;

	SampleMode sampleModeX = SampleMode::Clamp;
	SampleMode sampleModeY = SampleMode::Clamp;

	FilterMode minFilter = FilterMode::Point;
	FilterMode magFilter = FilterMode::Point;

	bool hasMipmaps = false;
};
