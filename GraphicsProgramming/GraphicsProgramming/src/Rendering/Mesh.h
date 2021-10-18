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


	inline void setVertices(std::vector<Vector3> v) { Vertices = v; }
	inline void setNormals(std::vector<Vector3> n) { Normals = n; }
	inline void setIndices(std::vector<int> i) { Indices = i; }

	// defined in DefaultMeshes.cpp
	static const Mesh Quad;
	static const Mesh Cube;
};
