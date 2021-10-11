#include "Light.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "RenderHelper.h"
#include "Transform.h"

void Light::render(unsigned int lightID, bool debugSphere)
{
	if (type == LightType::Invalid) return;


	glLightfv(lightID, GL_DIFFUSE, diffuseColor.ptr());
	glLightfv(lightID, GL_AMBIENT, ambientColor.ptr());
	glLightfv(lightID, GL_POSITION, homogeneousPos.ptr());

	if (type == LightType::Spot)
	{
		glLightfv(lightID, GL_SPOT_DIRECTION, spotDirection.ptr());
		glLightf(lightID, GL_SPOT_EXPONENT, spotExponent);
		glLightf(lightID, GL_SPOT_CUTOFF, spotCutoff);
	}

	if (debugSphere && type != LightType::Directional)
	{
		glDisable(GL_LIGHTING);
		float currentColor[4];
		glGetFloatv(GL_CURRENT_COLOR, currentColor);

		{
			Transform t(homogeneousPos.xyz, { 0, 0, 0 }, { 1, 1, 1 });

			glColor3f(1.0f, 1.0f, 1.0f);
			RenderHelper::drawSphere(0.05f);
		}

		glColor3fv(currentColor);
		glEnable(GL_LIGHTING);
	}
}

void Light::setType(LightType t)
{
	type = t;
	if (t == LightType::Invalid) return;

	homogeneousPos.w = t == LightType::Directional ? 0.0f : 1.0f;
}
