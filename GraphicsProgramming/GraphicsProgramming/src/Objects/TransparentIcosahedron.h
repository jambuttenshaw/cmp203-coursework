#pragma once

#include "Core/Color.h"
#include "TransparentObject.h"

#include <glm/glm.hpp>
#include <array>

// the difficulty of rendering a transparent 3D model is that all of its
// triangles need to be sorted in order of furthest to closest to the camera
class TransparentIcosahedron : public TransparentObject
{
public:
	TransparentIcosahedron();

	void Render(const glm::vec3& camPos);
	void RandomizeColours();
private:
	const float faceAlpha = 0.7f;

	// for simplicity of sorting and rendering individual triangles, 
	// we store all the faces of the icosahedron as seperate objects in an array
	// not very efficient to render but were only rendering 1 icosahedron...
	struct Face
	{
		Mesh mesh;
		glm::vec3 centroid{ 0.0f };
		Color color;
	};
	std::array<Face, 20> faces;
};