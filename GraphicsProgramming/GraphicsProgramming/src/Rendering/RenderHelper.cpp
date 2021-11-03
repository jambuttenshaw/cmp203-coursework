#include "RenderHelper.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


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
	glBegin(GL_TRIANGLES);

	for (auto i : mesh.Indices)
	{
		const auto& v = mesh.Vertices[i];
		glTexCoord2f(v.TexCoord.x, v.TexCoord.y);
		glNormal3f(v.Normal.x, v.Normal.y, v.Normal.z);
		glVertex3f(v.Position.x, v.Position.y, v.Position.z);
	}

	glEnd();
}

