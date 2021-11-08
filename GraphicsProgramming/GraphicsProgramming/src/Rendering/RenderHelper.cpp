#include "RenderHelper.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


bool RenderHelper::wireframe = false;


void RenderHelper::drawSphere(float radius, int slices, int stacks)
{
	glutSolidSphere(radius, slices, stacks);
}

void RenderHelper::drawQuad()
{
	drawMesh(Mesh::Quad);
}

void RenderHelper::drawUnitCube()
{
	drawMesh(Mesh::Cube);
}


void RenderHelper::drawMesh(const Mesh& mesh)
{
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &mesh.Vertices[0].Position);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &mesh.Vertices[0].Normal);

	if (mesh.MeshTexture != nullptr && !wireframe)
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &mesh.Vertices[0].TexCoord);
		mesh.MeshTexture->Bind();
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, &mesh.Indices[0]);

	glPopClientAttrib();
	glPopAttrib();
}

void RenderHelper::EnableWireframeMode()
{
	if (wireframe) return;

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	wireframe = true;
}

void RenderHelper::DisableWireframeMode()
{
	if (!wireframe) return;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopAttrib();

	wireframe = false;
}

void RenderHelper::ToggleWireframeMode()
{
	if (wireframe) DisableWireframeMode(); else EnableWireframeMode();
}
