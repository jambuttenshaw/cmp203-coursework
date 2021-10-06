#include "RenderHelper.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

void RenderHelper::drawSphere(float size, int slices, int stacks)
{
	gluSphere(gluNewQuadric(), size, slices, stacks);
}

void RenderHelper::drawSphere(float size, float r, float g, float b, int slices, int stacks)
{
	glColor3f(r, g, b);
	drawSphere(size, slices, stacks);
}
