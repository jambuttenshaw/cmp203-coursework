#pragma once

#include "Vector3.h"

class Transform
{
public:
	Transform(Vector3 translation, Vector3 rotation, Vector3 scale);
	~Transform();
};

