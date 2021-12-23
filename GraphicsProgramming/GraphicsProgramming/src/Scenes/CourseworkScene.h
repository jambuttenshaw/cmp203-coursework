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
	Light spotLight;
	Light panelLight;

	Texture* cubeTexture = nullptr;
	Texture* whiteWallTexture = nullptr;
	Texture* blackWallTexture = nullptr;
	Texture* concreteTexture = nullptr;

	Mesh hallWhitePlane;
	Mesh hallBlackPlane;
	Mesh backWallPlane;
	Mesh corridorWhitePlane;
	Mesh corridorWallPlane;
	Mesh corridorEndPlane;

	Mesh ceilingLight;

	GameObject cube;
	GameObject portalGun;
	GameObject cylinder;
	GameObject lightPanel;
	GameObject sphere;

	Material portalGunMat;
	Material ceilingLightMat;
	Material spotLightMat;
	Material lightPanelMat;
	Material plastic;

	std::vector<Mesh> shadowVolumes;

	float portalGunRotation = 0.0f;
};
