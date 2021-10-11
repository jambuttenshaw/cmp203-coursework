#pragma once

#include <functional>

#include "Mesh.h"


class GeometryHelper
{
public:
	GeometryHelper() = delete;

	static Mesh CreatePlane(size_t xSlices, size_t ySlices, std::function<float(float, float)> heightFunc = [](float, float) -> float {return 0; });
};

