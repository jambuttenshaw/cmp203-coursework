#include "Spotlight.h"

#include "Rendering/GeometryHelper.h"
#include "Core/Math.h"


void Spotlight::Setup(const glm::vec3& pos, float yaw, float pitch, const Color& color)
{
	*mMesh = GeometryHelper::LoadObj("models/spotlight.obj");

	mTransform->SetTranslation(pos);
	mTransform->SetRotation({ 0, yaw, pitch });
	mTransform->SetScale(glm::vec3(0.25f));

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
}
