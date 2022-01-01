#include "Spotlight.h"

#include "Rendering/GeometryHelper.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/Transformation.h"


void Spotlight::Setup(const glm::vec3& pos, float yaw, float pitch, const Color& color)
{
	// set up the models that make up this object
	mBody.GetMesh() = GeometryHelper::LoadObj("models/spotlight.obj");

	// setup the transform of the body of the spotlight
	mBody.GetTransform().SetTranslation(pos);
	mBody.GetTransform().SetRotation({ 0, yaw, pitch });
	mBody.GetTransform().SetScale(glm::vec3(0.25f));


	mHemisphere.GetMesh() = GeometryHelper::LoadObj("models/hemisphere.obj");

	// setup the hemisphere that sits on top of the spotlight to make it appear if its lit
	mHemisphere.GetTransform().SetTranslation({ 1.3f, 0, 0 });
	mHemisphere.GetTransform().SetRotation({ 0, 0, -90 });
	mHemisphere.GetTransform().SetScale({ 0.85f, 0.4f, 0.85f });

	// set up the light source
	mLight.setType(Light::LightType::Spot);

	mLight.setAmbientColor(0.1f);
	mLight.setDiffuseColor(color);
	mLight.setSpecularColor(color);
	mLight.setAttenuation({ 1.0f, 0.1f, 0 });

	// then calculate the direction the spotlight points in
	float yawRads = glm::radians(yaw);
	float pitchRads = glm::radians(pitch);

	glm::vec3 dir;
	dir.x = cosf(yawRads) * cosf(pitchRads);
	dir.y = sinf(pitchRads);
	dir.z = sinf(yawRads) * cosf(pitchRads);

	// setup spotlight
	mLight.setPosition(pos);
	mLight.setSpotDirection(dir);

	mLight.setSpotCutoff(50);
	mLight.setSpotExponent(4);

	// setup the materials
	emissiveMat.setAmbientAndDiffuse(Color::Black);
	emissiveMat.setEmission(color);

	bodyMat.setAmbientAndDiffuse(0.5f);
	bodyMat.setSpecular(1.0f);
	bodyMat.setShininess(120);
}

void Spotlight::Render() const
{
	// render the body and the hemisphere at the top of the spotlight
	{
		Transformation t(mBody);
		{
			// heirarchal transform: the hemisphere is rendered relative to the body of the spotlight
			Transformation t2(mHemisphere);

			emissiveMat.apply();
			RenderHelper::drawMesh(mHemisphere);
		}

		bodyMat.apply();
		RenderHelper::drawMesh(mBody);
	}
}

void Spotlight::SetColour(const Color& c)
{
	// sets the colour of the spotlight and emissive material
	mLight.setDiffuseColor(c);
	mLight.setSpecularColor(c);

	emissiveMat.setEmission(c);
}
