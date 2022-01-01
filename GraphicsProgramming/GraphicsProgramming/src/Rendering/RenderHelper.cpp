#include "RenderHelper.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Core/Color.h"

#include <cassert>


bool RenderHelper::wireframe = false;

void RenderHelper::drawSphere(float radius, int slices, int stacks)
{
	glutSolidSphere(radius, slices, stacks);
}

void RenderHelper::drawQuad()
{
	drawMesh(Mesh::Quad);
}

void RenderHelper::drawQuad(Texture* tex)
{
	// the default mesh Mesh::Quad is marked as const, so we can not modify the texture of it
	// however we can have our own default quad mesh, static to this function, that we can modify the texture of
	static Mesh texturedQuad = Mesh::Quad;

	assert(tex != nullptr);
	texturedQuad.MeshTexture = tex;
	drawMesh(texturedQuad);
}

void RenderHelper::drawUnitCube()
{
	drawMesh(Mesh::Cube);
}


void RenderHelper::drawMesh(const Mesh& mesh)
{
	// the meat of the entire rendering pipeline
	// can render any mesh object, with or without textures, in wireframe mode or not

	// save opengl state
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	// enable and assign vertex pointer
	// important: the stride is given as sizeof(Vertex). This is the offset in memory between one position, normal, or tex coordinate and the next
	// as the memory format is: pos, normal, texcoord, pos, normal, texcoord, ...
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &mesh.Vertices[0].Position);

	// enable and assign normal pointer
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &mesh.Vertices[0].Normal);

	// if this mesh should be drawn with a texture
	if (mesh.MeshTexture != nullptr && !wireframe)
	{
		// enable textures and tex coord array
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		// assign pointer and bind texture
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mesh.Vertices[0].TexCoord);
		mesh.MeshTexture->Bind();
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	// finally use the index array to render the mesh
	// we always use triangles. triangles are cool.
	glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, &mesh.Indices[0]);

	glPopClientAttrib();
	glPopAttrib();
}

void RenderHelper::drawMeshWireframeOverlay(const Mesh& mesh)
{
	// draw normally in wireframe mode
	if (wireframe)
	{
		drawMesh(mesh);
		return;
	}

	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_LIGHTING_BIT);

	// draw solid, filled objects with offset applied
	glPolygonOffset(1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);

	// draw the mesh once, this will draw the mesh normally as a solid object
	drawMesh(mesh);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_POLYGON_OFFSET_FILL);

	// draw solid black lines that are not affected by lights, textures or materials
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_COLOR_MATERIAL);
	glColor3fv(Color::Black.ptr());

	// draw the mesh a second time, and this will draw the wireframe overlay
	drawMesh(mesh);

	glPopAttrib();
}

void RenderHelper::EnableWireframeMode()
{
	if (wireframe) return;

	// disable lighting and textures, and set the polygon mode
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	wireframe = true;
}

void RenderHelper::DisableWireframeMode()
{
	if (!wireframe) return;
	// set the polygon mode and push from the attrib stack
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopAttrib();

	wireframe = false;
}

void RenderHelper::ToggleWireframeMode()
{
	if (wireframe) DisableWireframeMode(); else EnableWireframeMode();
}
