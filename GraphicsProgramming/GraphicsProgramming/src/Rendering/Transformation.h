#pragma once

#include <glm/glm.hpp>

#include "Transform.h"
#include "Objects/GameObject.h"

// a tidy way of handling the matrix stack
class Transformation
{
public:
	// a variety of different constructors to make it easy to work with
	// most of them delegate to the TRS constructor
	Transformation();
	Transformation(const glm::vec3& translation);
	Transformation(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

	Transformation(const Transform& t);
	Transformation(const GameObject& go);
	Transformation(const glm::mat4& matrix);

	~Transformation();
};

