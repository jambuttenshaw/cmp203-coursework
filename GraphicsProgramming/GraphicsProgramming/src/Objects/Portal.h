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

	void Update(float dt, Input* in, Camera* traveller);
	void Render();

	inline void SetLinkedPortal(Portal* portal) { mLinkedPortal = portal; }
	inline Transform& GetTransform() { return mTransform; }

	inline const Mesh& GetFrameModel() const { return mFrameModel; }
	
private:
	Mesh mScreenModel;
	Mesh mFrameModel;

	Portal* mLinkedPortal = nullptr;
	PortalScene* mSceneToRender = nullptr;

	Transform mTransform;


private:
	static float sLastSideOfPortal;
	static bool sPortalRenderInProgress;
	static bool sPortalUpdateInProgress;

};