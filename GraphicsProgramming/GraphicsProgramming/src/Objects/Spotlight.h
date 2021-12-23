#pragma once

#include "GameObject.h"
#include "Rendering/Light.h"
#include "Rendering/Material.h"


class Spotlight
{
public:
	Spotlight() = default;
	
	void Setup(const glm::vec3& pos, float yaw, float pitch, const Color& color);
	Light* GetLight() { return &mLight; }

	void Render() const;

private:
	Light mLight;

	GameObject mBody;
	GameObject mHemisphere;

	Material emissiveMat;
	Material bodyMat;
};