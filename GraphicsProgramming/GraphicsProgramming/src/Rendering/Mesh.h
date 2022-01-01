#pragma once

#include <vector>
#include <cassert>

#include <glm/glm.hpp>
#include "Texture.h"

// a vertex has a position, a normal, and a texture coordinate
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};


class Mesh
{
public:
	// a mesh is just a collection of vertices, and a collection of indices saying which vertices are connected to each other
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	// a mesh also has a pointer to a texture to be rendered onto the mesh
	// note: a mesh does NOT own its texture. you should delete it yourself elsewhere!
	Texture* MeshTexture = nullptr;

	// defined in DefaultMeshes.cpp
	static const Mesh Quad;
	static const Mesh Cube;

	Mesh() = default;

	// a few different constructors for creating meshes in different ways
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		: Vertices(vertices), Indices(indices) {}

	// create a mesh from 4 different vectors
	Mesh(std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texCoords,
		std::vector<unsigned int> indices)
		: Indices(indices)
	{
		// make sure the mesh data is actually valid
		assert((positions.size() == normals.size()) && (positions.size() == texCoords.size()) && "Invalid model data!");

		// we can resize vertices in one go
		Vertices.resize(positions.size());

		for (size_t vertex = 0; vertex < positions.size(); vertex++)
		{
			// construct a Vertex object for each vertex in the mesh
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

