#include "Light.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "RenderHelper.h"
#include "Transformation.h"

bool Light::debugSpheresOnAllLights = false;


void Light::render(unsigned int lightID, bool debugSphere)
{
	if (type == LightType::Invalid) return;

	glEnable(lightID);

	glLightfv(lightID, GL_DIFFUSE, diffuseColor.ptr());
	glLightfv(lightID, GL_AMBIENT, ambientColor.ptr());
	glLightfv(lightID, GL_SPECULAR, specularColor.ptr());
	glLightfv(lightID, GL_POSITION, homogeneousPos.ptr());

	if (type == LightType::Spot)
	{
		glLightfv(lightID, GL_SPOT_DIRECTION, &spotDirection.x);
		glLightf(lightID, GL_SPOT_EXPONENT, spotExponent);
		glLightf(lightID, GL_SPOT_CUTOFF, spotCutoff);
	}

	glLightf(lightID, GL_CONSTANT_ATTENUATION, attentuation.x);
	glLightf(lightID, GL_LINEAR_ATTENUATION, attentuation.y);
	glLightf(lightID, GL_QUADRATIC_ATTENUATION, attentuation.z);


	// debug sphere

	if ((debugSphere || debugSpheresOnAllLights) && type != LightType::Directional)
	{
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		float currentColor[4];
		glGetFloatv(GL_CURRENT_COLOR, currentColor);

		{
			Transformation t(homogeneousPos.xyz, { 0, 0, 0 }, { 1, 1, 1 });

			glColor3f(1.0f, 1.0f, 1.0f);
			RenderHelper::drawSphere(0.05f);
		}

		glColor3fv(currentColor);
		glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
	}
}

void Light::setType(LightType t)
{
	type = t;
	if (t == LightType::Invalid) return;

	homogeneousPos.w = t == LightType::Directional ? 0.0f : 1.0f;
}
