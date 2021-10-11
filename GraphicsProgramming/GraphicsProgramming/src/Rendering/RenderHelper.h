#pragma once

#include "Core/Vector3.h"
#include "Mesh.h"

class RenderHelper
{
public:
	RenderHelper() = delete;

	static void drawSphere(float size, int slices = 20, int stacks = 20);
	static void drawSphere(float size, float r, float g, float b, int slices = 20, int stacks = 20);

	static void drawUnitCube();

	static void drawMesh(const Mesh& mesh);

private:
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
	};
	static const Vertex cubeVertices[];
	static const size_t cubeIndices[];

};

