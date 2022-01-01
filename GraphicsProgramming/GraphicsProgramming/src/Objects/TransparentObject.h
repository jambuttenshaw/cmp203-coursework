#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "GameObject.h"

#include <functional>

// represents a transparent object
// it is a class instead of a struct because classes may want to derive from this to add in behaviour
class TransparentObject
{
public:
	// properties of the object
	GameObject gameObject;
	const Material* mat;

	// how to render the object
	std::function<void(void)> renderObject;
};
