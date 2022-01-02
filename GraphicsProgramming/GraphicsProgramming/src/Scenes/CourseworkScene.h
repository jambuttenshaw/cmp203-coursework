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
	// all the lights in the scene
	Light pointLight;
	Light spotLight;
	Light panelLight;

	// all the textures used in this scene
	Texture* cubeTexture = nullptr;
	Texture* whiteWallTexture = nullptr;
	Texture* blackWallTexture = nullptr;
	Texture* concreteTexture = nullptr;

	// the meshes making up the room
	Mesh hallWhitePlane;
	Mesh hallBlackPlane;
	Mesh backWallPlane;
	Mesh corridorWhitePlane;
	Mesh corridorWallPlane;
	Mesh corridorEndPlane;

	//  the ceiling light is not a gameobject as the same mesh is rendered in multiple locations
	Mesh ceilingLight;

	// a bunch of game objects in the scene
	GameObject cube;
	GameObject portalGun;
	GameObject cylinder;
	GameObject lightPanel;
	GameObject sphere;

	// all materials used in the scene
	Material portalGunMat;
	Material ceilingLightMat;
	Material spotLightMat;
	Material lightPanelMat;
	Material plastic;

	// all shadow volumes to render in the scene are in this container
	std::vector<Mesh> shadowVolumes;

	// a parameter used to rotate the portal gun
	float portalGunRotation = 0.0f;

	size_t boxShadowVolumeIndex = 0;
	float boxPos = 0.0f;
};
