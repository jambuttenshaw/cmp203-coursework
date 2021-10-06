#pragma once

#include "Mesh.h"

class GeometryHelper
{
public:
	GeometryHelper() = delete;

	static Mesh CreatePlane(size_t xSlices, size_t ySlices);
};

