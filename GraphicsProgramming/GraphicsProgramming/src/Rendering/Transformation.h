#pragma once

#include <glm/glm.hpp>

class Transformation
{
public:
	Transformation();
	Transformation(glm::vec3 translation);
	Transformation(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	~Transformation();
};

