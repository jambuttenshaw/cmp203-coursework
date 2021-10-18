#pragma once

#include <functional>

#include "Mesh.h"


class GeometryHelper
{
public:
	class HeightFuncs
	{
	public:
		// default height funcs
		static float Flat(float x, float y) { return 0; }
		static float PerlinNoiseTerrain(float x, float z);
	};

public:
	GeometryHelper() = delete;

	static Mesh CreatePlane(size_t xSlices, size_t ySlices,
							float uScale = 1.0f, float vScale = 1.0f,
							std::function<float(float, float)> heightFunc = HeightFuncs::Flat);
};
