#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"

#include "Rendering/Transform.h"

#include "Core/Input.h"

#include <glm/glm.hpp>


class Scene;

class Portal
{
public:
	Portal(Scene* sceneToRender);
	~Portal() = default;

	void TestForTravelling(Input* in, const glm::vec3& travellerPosition);
	void Render();

	inline void SetLinkedPortal(Portal* portal) { mLinkedPortal = portal; }
	inline void SetTexture(Texture* tex) { mFrameModel.MeshTexture = tex; }

	inline Transform& GetTransform() { return mTransform; }
	
private:
	Mesh mFrameModel;
	Mesh mScreenModel;

	Portal* mLinkedPortal = nullptr;
	Scene* mSceneToRender = nullptr;

	Transform mTransform;

	// for tracking traveller
	int mLastSideOfPortal = 0;

private:
	static bool sPortalRenderInProgress;

};