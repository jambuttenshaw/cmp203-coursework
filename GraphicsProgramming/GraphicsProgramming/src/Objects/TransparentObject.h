#pragma once

#include "Rendering/Mesh.h"
#include "GameObject.h"

#include <functional>


struct TransparentObject
{
	// properties of the object
	GameObject gameObject;
	const Material* mat;

	// how to render the object
	std::function<void(void)> renderObject;
};
