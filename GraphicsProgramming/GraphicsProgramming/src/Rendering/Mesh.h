#pragma once

#include <vector>

#include "Core/Vector3.h"
#include "Core/Vector2.h"


struct Mesh
{
	std::vector<Vector3> Vertices;
	std::vector<Vector3> Normals;
	std::vector<Vector2> TexCoords;
	std::vector<int> Indices;

	// defined in DefaultMeshes.cpp
	static const Mesh Quad;
	static const Mesh Cube;
};
