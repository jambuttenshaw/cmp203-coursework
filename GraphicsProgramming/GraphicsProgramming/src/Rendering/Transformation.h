#pragma once

#include <glm/glm.hpp>

#include "Transform.h"
#include "Objects/GameObject.h"

class Transformation
{
public:
	Transformation();
	Transformation(const glm::vec3& translation);
	Transformation(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

	Transformation(const Transform& t);
	Transformation(const GameObject& go);
	Transformation(const glm::mat4& matrix);

	~Transformation();
};

