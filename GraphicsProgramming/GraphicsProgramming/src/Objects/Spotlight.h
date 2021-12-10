#pragma once

#include "GameObject.h"
#include "Rendering/Light.h"


class Spotlight : public GameObject
{
public:
	Spotlight() = default;
	
	void Setup(const glm::vec3& pos, float yaw, float pitch, const Color& color);
	Light* GetLight() { return &mLight; }

private:
	Light mLight;
};