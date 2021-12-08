#pragma once

#include "Objects/PortalScene.h"
#include "Objects/Portal.h"

#include <vector>


class CourseworkScene : public PortalScene
{
public:
	CourseworkScene() = default;
	~CourseworkScene();

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRenderObjects() override;
	virtual void OnRenderShadowVolumes() override;

	virtual void SetExitPortal(Portal* p) override;
	inline virtual Portal* GetEntryPortal() const override { return mExitPortal; }

private:
	Light pointLight;

	Texture* cubeTexture = nullptr;
	Texture* whiteWallTexture = nullptr;
	Texture* blackWallTexture = nullptr;

	Mesh whitePlane;
	Mesh blackPlane;

	GameObject cube;
	GameObject portalGun;

	Material portalGunMat;

	std::vector<Mesh> shadowVolumes;
};
