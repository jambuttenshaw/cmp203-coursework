#include "Transformation.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <glm/gtc/type_ptr.hpp>

Transformation::Transformation()
	: Transformation({0, 0, 0}, {0, 0, 0}, {1, 1, 1})
{
}

Transformation::Transformation(const glm::vec3& translation)
	: Transformation(translation, {0, 0, 0}, {1, 1, 1})
{
}

Transformation::Transformation(const glm::vec3& t, const glm::vec3& r, const glm::vec3& s)
{
	glPushMatrix();

	glTranslatef(t.x, t.y, t.z);
	glRotatef(r.x, 1, 0, 0);
	glRotatef(r.y, 0, 1, 0);
	glRotatef(r.z, 0, 0, 1);
	glScalef(s.x, s.y, s.z);
}

Transformation::Transformation(const Transform& t)
	: Transformation(t.GetTranslation(), t.GetRotation(), t.GetScale())
{
}

Transformation::Transformation(const GameObject& go)
	: Transformation(go.GetTransform())
{
}

Transformation::Transformation(const glm::mat4& matrix)
{
	// this doesnt work :(
	glPushMatrix();
	glLoadMatrixf(glm::value_ptr(matrix));
}

Transformation::~Transformation()
{
	glPopMatrix();
}
