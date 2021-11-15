#pragma once

#include <vector>
#include <cassert>

#include "Core/Vector3.h"
#include "Core/Vector2.h"

#include "Texture.h"

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexCoord;
};


class Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	Texture* MeshTexture = nullptr;

	// defined in DefaultMeshes.cpp
	static const Mesh Quad;
	static const Mesh Cube;

	Mesh() = default;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		: Vertices(vertices), Indices(indices) {}

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

