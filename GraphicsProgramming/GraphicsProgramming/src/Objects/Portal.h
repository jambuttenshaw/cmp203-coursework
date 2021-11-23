#pragma once

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"

class Scene;

class Portal
{
public:
	Portal(Scene* sceneToRender);
	~Portal() = default;

	void Render();

	inline void SetLinkedPortal(Portal* portal) { mLinkedPortal = portal; }
	inline void SetTexture(Texture* tex) { mFrameModel.MeshTexture = tex; }


private:
	Mesh mFrameModel;
	Mesh mScreenModel;

	Portal* mLinkedPortal = nullptr;
	Scene* mSceneToRender = nullptr;

private:
	static bool sPortalRenderInProgress;

};