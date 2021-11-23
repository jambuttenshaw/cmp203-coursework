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


	void SetPosition(const glm::vec3& pos);


private:
	Mesh mFrameModel;
	Mesh mScreenModel;

	Portal* mLinkedPortal = nullptr;
	Scene* mSceneToRender = nullptr;

	// identity matrix
	glm::mat4 mLocalToWorld{ 1.0f };

private:
	static bool sPortalRenderInProgress;

};