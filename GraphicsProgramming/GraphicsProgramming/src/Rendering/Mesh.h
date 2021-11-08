#pragma once

#include <vector>
#include <cassert>

#include "Core/Vector3.h"
#include "Core/Vector2.h"


struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoord;
};


struct Mesh
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	// defined in DefaultMeshes.cpp
	static const Mesh Quad;
	static const Mesh Cube;

	Mesh() = default;

	Mesh(std::vector<Vector3> positions,
		std::vector<Vector3> normals,
		std::vector<Vector2> texCoords,
		std::vector<unsigned int> indices)
		: Indices(indices)
	{
		assert((positions.size() == normals.size()) && (positions.size() == texCoords.size()) && "Invalid model data!");

		Vertices.resize(positions.size());

		for (size_t vertex = 0; vertex < positions.size(); vertex++)
		{
			Vertices[vertex] = { positions[vertex], normals[vertex], texCoords[vertex] };
		}
	}
};

