#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Rendering/Transform.h"

#include <functional>


struct TransparentObject
{
	// properties of the object
	Mesh mesh;
	Material mat;
	Transform transform;

	// how to render the object
	std::function<void(void)> renderObject;
};
