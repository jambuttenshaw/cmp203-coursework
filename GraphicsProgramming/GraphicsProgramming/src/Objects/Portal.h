#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"

#include "Rendering/Transform.h"
#include "Rendering/Camera.h"

#include "Core/Input.h"

#include <glm/glm.hpp>

// forward declaration to avoid circular includes
class PortalScene;

class Portal
{
public:
	Portal(PortalScene* sceneToRender);
	~Portal() = default;

	// update tracks when the traveller should move through the portal, as well as updates the scene that can be seen through the portal
	void Update(float dt, Input* in, Camera* traveller);
	// render draws the portal and the scene on the other side of the portal
	void Render();

	inline void SetLinkedPortal(Portal* portal) { mLinkedPortal = portal; }
	inline Transform& GetTransform() { return mTransform; }

	// used for creating shadow volumes of the portals frame
	inline const Mesh& GetFrameModel() const { return mFrameModel; }
	
private:
	// the meshes used in rendering the portal
	Mesh mScreenModel;
	Mesh mFrameModel;

	Portal* mLinkedPortal = nullptr;
	PortalScene* mSceneToRender = nullptr;

	Transform mTransform;


private:
	// portals need to share some global state between each other, so portals dont try to do things when other
	// portals are also doing things... this is easily handled by using static variables (but would not be thread-safe in a multithreaded application)
	static float sLastSideOfPortal;
	static bool sPortalRenderInProgress;
	static bool sPortalUpdateInProgress;

};