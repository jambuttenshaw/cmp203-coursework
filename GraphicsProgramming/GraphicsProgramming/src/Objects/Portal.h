#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"

#include "Rendering/Transform.h"

#include <glm/glm.hpp>


class Scene;

class Portal
{
public:
	Portal(Scene* sceneToRender);
	~Portal() = default;

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

private:
	static bool sPortalRenderInProgress;

};