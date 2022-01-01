#include "Transformation.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <glm/gtc/type_ptr.hpp>

// push identity matrix onto the matrix stack
Transformation::Transformation()
	: Transformation({0, 0, 0}, {0, 0, 0}, {1, 1, 1})
{
}

// translate by an amount
Transformation::Transformation(const glm::vec3& translation)
	: Transformation(translation, {0, 0, 0}, {1, 1, 1})
{
}

// translate, rotate and scale by an amount
Transformation::Transformation(const glm::vec3& t, const glm::vec3& r, const glm::vec3& s)
{
	glPushMatrix();

	glTranslatef(t.x, t.y, t.z);
	// this coursework assumes the convention of rotating about x axis, then y, then z
	glRotatef(r.x, 1, 0, 0);
	glRotatef(r.y, 0, 1, 0);
	glRotatef(r.z, 0, 0, 1);
	glScalef(s.x, s.y, s.z);
}

// apply a transform class to the matrix stack
Transformation::Transformation(const Transform& t)
	: Transformation(t.GetTranslation(), t.GetRotation(), t.GetScale())
{
}

// apply a transform from a game object
Transformation::Transformation(const GameObject& go)
	: Transformation(go.GetTransform())
{
}

// add the matrix parameter to the matrix stack
Transformation::Transformation(const glm::mat4& matrix)
{
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(matrix));
}

Transformation::~Transformation()
{
	glPopMatrix();
}
