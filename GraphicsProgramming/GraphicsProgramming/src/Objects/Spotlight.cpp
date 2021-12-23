#include "Spotlight.h"

#include "Core/Math.h"
#include "Rendering/GeometryHelper.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/Transformation.h"


void Spotlight::Setup(const glm::vec3& pos, float yaw, float pitch, const Color& color)
{
	mBody.GetMesh() = GeometryHelper::LoadObj("models/spotlight.obj");

	mBody.GetTransform().SetTranslation(pos);
	mBody.GetTransform().SetRotation({ 0, yaw, pitch });
	mBody.GetTransform().SetScale(glm::vec3(0.25f));


	mHemisphere.GetMesh() = GeometryHelper::LoadObj("models/hemisphere.obj");

	mHemisphere.GetTransform().SetTranslation({ 1.3f, 0, 0 });
	mHemisphere.GetTransform().SetRotation({ 0, 0, -90 });
	mHemisphere.GetTransform().SetScale({ 0.85f, 0.4f, 0.85f });


	mLight.setType(Light::LightType::Spot);

	mLight.setAmbientColor(0.1f);
	mLight.setDiffuseColor(color);
	mLight.setSpecularColor(color);

	float yawRads = Math::radians(yaw);
	float pitchRads = Math::radians(pitch);

	glm::vec3 dir;
	dir.x = cosf(yawRads) * cosf(pitchRads);
	dir.y = sinf(pitchRads);
	dir.z = sinf(yawRads) * cosf(pitchRads);

	mLight.setPosition(pos);
	mLight.setSpotDirection(dir);

	mLight.setSpotCutoff(50);
	mLight.setSpotExponent(4);


	emissiveMat.setAmbientAndDiffuse(Color::Black);
	emissiveMat.setEmission(color);

	bodyMat.setAmbientAndDiffuse(0.5f);
	bodyMat.setSpecular(1.0f);
	bodyMat.setShininess(120);
}

void Spotlight::Render() const
{
	{
		Transformation t(mBody);
		{
			Transformation t2(mHemisphere);

			emissiveMat.apply();
			RenderHelper::drawMesh(mHemisphere);
		}

		bodyMat.apply();
		RenderHelper::drawMesh(mBody);
	}
}
