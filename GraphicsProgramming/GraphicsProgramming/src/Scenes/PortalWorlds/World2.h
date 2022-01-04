#pragma once

#include "Objects/PortalScene.h"
#include "Objects/Portal.h"
#include "Objects/TransparentIcosahedron.h"


class World2 : public PortalScene
{
public:
	World2() = default;
	~World2();

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRenderObjects() override;
	virtual void OnRenderShadowVolumes() override;

	virtual void SetExitPortal(Portal* p) override;
	inline virtual Portal* GetEntryPortal() const override { return mExitPortal; }

private:
	// a seperate camera for viewing the icosahedron
	Camera* orbitCamera = nullptr;
	const float orbitRadius = 3.0f;
	float oldFOV = 0.0f;

	// a directional light to illuminate the scene
	Light dirLight;

	// all textures in the scene
	Texture* sandTexture = nullptr;
	Texture* windowTexture = nullptr;

	// objects in the scene
	GameObject groundPlane;

	// transparent objects
	TransparentObject window1;
	TransparentObject window2;

	TransparentIcosahedron icosahedron;
	// the sphere inside the icosahedron
	GameObject sphere; 

	// an awesome dude
	GameObject model;

	// stands to display the things on
	Mesh stand;
	Transform modelStandTransform;
	Transform icosahedronStandTransform;

	// all shadows in the scene
	std::vector<Mesh> shadowVolumes;

	// materials in the scene
	Material transparentMat;

	// a parameter to control animated objects
	float t = 0;
};
