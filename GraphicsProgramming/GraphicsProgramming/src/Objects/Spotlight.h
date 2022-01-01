#pragma once

#include "GameObject.h"
#include "Rendering/Light.h"
#include "Rendering/Material.h"

// specialized object used in one of the worlds
// designed to represent a spotlight, as would be seen on a stage
class Spotlight
{
public:
	Spotlight() = default;
	
	// sets position, angle and colour
	void Setup(const glm::vec3& pos, float yaw, float pitch, const Color& color);
	// so that the light can be registered in the scene
	Light* GetLight() { return &mLight; }

	// draw the meshes that make up this object
	void Render() const;

	// set the colour of the light and emissive material
	void SetColour(const Color& c);

private:
	Light mLight;

	GameObject mBody;
	GameObject mHemisphere;

	Material emissiveMat;
	Material bodyMat;
};