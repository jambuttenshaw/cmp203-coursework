#pragma once

#include "Core/Color.h"
#include "TransparentObject.h"

#include <glm/glm.hpp>
#include <array>


class TransparentIcosahedron : public TransparentObject
{
public:
	TransparentIcosahedron();

	void Render(const glm::vec3& camPos);
	void RandomizeColours();
private:
	const float faceAlpha = 0.7f;

	struct Face
	{
		Mesh mesh;
		glm::vec3 centroid{ 0.0f };
		Color color;
	};
	std::array<Face, 20> faces;
};