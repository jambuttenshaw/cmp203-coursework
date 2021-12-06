#pragma once

#include <glm/glm.hpp>
#include "Mesh.h"

class RenderHelper
{
public:
	RenderHelper() = delete;

	static void drawSphere(float radius, int slices = 20, int stacks = 20);

	static void drawQuad();

	static void drawUnitCube();

	static void drawMesh(const Mesh& mesh);
	static void drawMeshWireframeOverlay(const Mesh& mesh);



	static void EnableWireframeMode();
	static void DisableWireframeMode();
	static void ToggleWireframeMode();

private:
	static bool wireframe;
};   

