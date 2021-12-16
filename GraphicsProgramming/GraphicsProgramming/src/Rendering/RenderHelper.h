#pragma once

#include <glm/glm.hpp>
#include "Mesh.h"
#include "Objects/GameObject.h"

class RenderHelper
{
public:
	RenderHelper() = delete;

	static void drawSphere(float radius, int slices = 20, int stacks = 20);

	static void drawQuad();
	static void drawQuad(Texture* tex);

	static void drawUnitCube();

	inline static void drawMesh(const GameObject& go) { drawMesh(go.GetMesh()); }
	static void drawMesh(const Mesh& mesh);
	inline static void drawMeshWireframeOverlay(const GameObject& go) { drawMeshWireframeOverlay(go.GetMesh()); }
	static void drawMeshWireframeOverlay(const Mesh& mesh);



	static void EnableWireframeMode();
	static void DisableWireframeMode();
	static void ToggleWireframeMode();

private:
	static bool wireframe;
};   

