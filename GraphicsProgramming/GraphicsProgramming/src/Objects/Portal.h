#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"

#include "Rendering/Transform.h"
#include "Rendering/Camera.h"

#include "Core/Input.h"

#include <glm/glm.hpp>


class PortalScene;

class Portal
{
public:
	Portal(PortalScene* sceneToRender);
	~Portal() = default;

	void TestForTravelling(Input* in, Camera* traveller);
	void Render();

	inline void SetLinkedPortal(Portal* portal) { mLinkedPortal = portal; }
	inline void SetTexture(Texture* tex) { mFrameModel.MeshTexture = tex; }

	inline Transform& GetTransform() { return mTransform; }
	
private:
	inline void SetSideOfPortal(float s) { mLastSideOfPortal = s; }


	void SetClippingPlanes(float nearPlane, float farPlane);


private:
	Mesh mFrameModel;
	Mesh mScreenModel;

	Portal* mLinkedPortal = nullptr;
	PortalScene* mSceneToRender = nullptr;

	Transform mTransform;

	// for tracking traveller
	float mLastSideOfPortal = 0.0f;

private:
	static bool sPortalRenderInProgress;

};