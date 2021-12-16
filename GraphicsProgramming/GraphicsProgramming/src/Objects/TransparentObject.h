#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "GameObject.h"

#include <functional>


class TransparentObject
{
public:
	// properties of the object
	GameObject gameObject;
	const Material* mat;

	// how to render the object
	std::function<void(void)> renderObject;
};
