#include "Transform.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

Transform::Transform(Vector3 t, Vector3 r, Vector3 s)
{
	glPushMatrix();

	glTranslatef(t.x, t.y, t.z);
	glRotatef(r.x, 1, 0, 0);
	glRotatef(r.y, 0, 1, 0);
	glRotatef(r.z, 0, 0, 1);
	glScalef(s.x, s.y, s.z);
}

Transform::~Transform()
{
	glPopMatrix();
}
