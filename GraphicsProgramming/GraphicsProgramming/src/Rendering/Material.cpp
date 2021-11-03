#include "Material.h"
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

void Material::apply()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.ptr());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.ptr());
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular.ptr());
	glMaterialfv(GL_FRONT, GL_EMISSION, emmission.ptr());
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
