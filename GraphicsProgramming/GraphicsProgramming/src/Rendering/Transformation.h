#pragma once

#include "Core/Vector3.h"

class Transformation
{
public:
	Transformation();
	Transformation(Vector3 translation);
	Transformation(Vector3 translation, Vector3 rotation, Vector3 scale);
	~Transformation();
};

