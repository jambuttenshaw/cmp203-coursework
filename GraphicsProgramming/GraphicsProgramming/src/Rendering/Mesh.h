#pragma once

#include <vector>
#include <cassert>

#include <glm/glm.hpp>
#include "Texture.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
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

	Mesh(std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texCoords,
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

	// for meshes without textures
	Mesh(std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<unsigned int> indices)
		: Indices(indices)
	{
		assert((positions.size() == normals.size()) && "Invalid model data!");

		Vertices.resize(positions.size());

		for (size_t vertex = 0; vertex < positions.size(); vertex++)
		{
			Vertices[vertex] = { positions[vertex], normals[vertex], { 0, 0} };
		}
	}
};

