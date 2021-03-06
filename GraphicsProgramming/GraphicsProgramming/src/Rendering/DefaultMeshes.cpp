#include "Mesh.h"


// a lot of data to define a simple quad and unit cube

const Mesh Mesh::Quad{
	// Vertices
	{
		
		{ -0.5f, 0,  0.5f },
		{  0.5f, 0,  0.5f },
		{  0.5f, 0, -0.5f },
		{ -0.5f, 0, -0.5f }

	},

	// Normals
	{
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 }
	},

	// Tex Coords
	{
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 }
	},

	// Indices
	{
		0, 1, 2,
		0, 2, 3
	}
};


const Mesh Mesh::Cube{
	// Vertices 
	{
		// front face
		{ -0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },

		// back face
		{  0.5f,  0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },

		// left face
		{ -0.5f,  0.5f, -0.5f },
		{ -0.5f,  0.5f,  0.5f },
		{ -0.5f, -0.5f,  0.5f },
		{ -0.5f, -0.5f, -0.5f },

		// right face
		{  0.5f,  0.5f,  0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{  0.5f, -0.5f,  0.5f },

		// top face				  
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f, -0.5f },
		{  0.5f,  0.5f,  0.5f },
		{ -0.5f,  0.5f,  0.5f },

		// bottom face
		{ -0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f,  0.5f },
		{  0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f }
	},

	// Normals
	{
		// front face
		{ 0, 0, 1 },
		{ 0, 0, 1 },
		{ 0, 0, 1 },
		{ 0, 0, 1 },

		// back face
		{ 0, 0, -1 },
		{ 0, 0, -1 },
		{ 0, 0, -1 },
		{ 0, 0, -1 },

		// left face
		{ -1, 0, 0 },
		{ -1, 0, 0 },
		{ -1, 0, 0 },
		{ -1, 0, 0 },

		// right face
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },
		{ 1, 0, 0 },

		// top face
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },
		{ 0, 1, 0 },

		// bottom face
		{ 0, -1, 0 },
		{ 0, -1, 0 },
		{ 0, -1, 0 },
		{ 0, -1, 0 }
	},

	// Tex Coords
	{
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 }
	},

	// Indices
	{
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 5, 6,
		4, 6, 7,
		// left face
		8, 9, 10,
		8, 10, 11,
		// right face
		12, 13, 14,
		12, 14, 15,
		// top face
		16, 17, 18,
		16, 18, 19,
		// bottom face
		20, 21, 22,
		20, 22, 23
	}
};
