#pragma once

#include <glm/glm.hpp>
#include "Mesh.h"
#include "Objects/GameObject.h"

class RenderHelper
{
public:
	// static class: do not instantiate
	RenderHelper() = delete;

	// draw a default glut sphere
	static void drawSphere(float radius, int slices = 20, int stacks = 20);

	// draw a 1x1 quad
	static void drawQuad();
	// draw a 1x1 quad with a texture
	static void drawQuad(Texture* tex);

	// draw a 1x1x1 unit cube
	static void drawUnitCube();

	// draw a mesh, from either a Mesh object or a GameObject
	inline static void drawMesh(const GameObject& go) { drawMesh(go.GetMesh()); }
	static void drawMesh(const Mesh& mesh);
	// draw a mesh and its wireframe on top of it
	inline static void drawMeshWireframeOverlay(const GameObject& go) { drawMeshWireframeOverlay(go.GetMesh()); }
	static void drawMeshWireframeOverlay(const Mesh& mesh);


	// control the state of wireframe mode
	static void EnableWireframeMode();
	static void DisableWireframeMode();
	static void ToggleWireframeMode();

private:
	static bool wireframe;
};   

