#include "Light.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "RenderHelper.h"
#include "Transformation.h"

// handy override to just enable debug spheres for everything
bool Light::debugSpheresOnAllLights = false;


void Light::render(unsigned int lightID, bool debugSphere) const
{
	// just in case...
	if (type == LightType::Invalid) return;

	// enable the light
	glEnable(lightID);

	// assign all properties of the light
	glLightfv(lightID, GL_DIFFUSE, diffuseColor.ptr());
	glLightfv(lightID, GL_AMBIENT, ambientColor.ptr());
	glLightfv(lightID, GL_SPECULAR, specularColor.ptr());
	glLightfv(lightID, GL_POSITION, homogeneousPos.ptr());

	// even if this light isnt a spotlight we still have to assign the default values to the spotlight properties
	glLightfv(lightID, GL_SPOT_DIRECTION, &spotDirection.x);
	glLightf(lightID, GL_SPOT_EXPONENT, spotExponent);
	glLightf(lightID, GL_SPOT_CUTOFF, spotCutoff);

	glLightf(lightID, GL_CONSTANT_ATTENUATION, attentuation.x);
	glLightf(lightID, GL_LINEAR_ATTENUATION, attentuation.y);
	glLightf(lightID, GL_QUADRATIC_ATTENUATION, attentuation.z);


	// debug sphere

	if ((debugSphere || debugSpheresOnAllLights) && type != LightType::Directional)
	{
		// should be unlit
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		
		{
			Transformation t(homogeneousPos.xyz, { 0, 0, 0 }, { 1, 1, 1 });
			RenderHelper::drawSphere(0.05f);
		}

		glPopAttrib();
	}
}

void Light::setType(LightType t)
{
	type = t;
	if (t == LightType::Invalid) return;

	// when switching between directional lights we need to set the w component to 0
	homogeneousPos.w = t == LightType::Directional ? 0.0f : 1.0f;
}
