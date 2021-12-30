#pragma once

#include <glm/glm.hpp>

#include "Rendering/Texture.h"
#include "Rendering/RenderHelper.h"

#include "Rendering/Transform.h"
#include "TransparentObject.h"

class Billboard : public TransparentObject
{
public:
	// a billboard is just a texture on a quad that rotates to face the camera
	// I have used cylindrical billboarding
	Billboard(Texture* tex);

	inline Transform& GetTransform() { return gameObject.GetTransform(); }

	void Face(const glm::vec3& pos);
	void Render();

private:
	Texture* mTex;
};