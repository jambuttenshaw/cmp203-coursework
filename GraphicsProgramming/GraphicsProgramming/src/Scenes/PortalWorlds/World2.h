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
	Camera* orbitCamera = nullptr;
	const float orbitRadius = 3.0f;
	float oldFOV = 0.0f;

	Light dirLight;

	Texture* sandTexture = nullptr;
	Texture* windowTexture = nullptr;

	GameObject groundPlane;

	TransparentObject window1;
	TransparentObject window2;

	TransparentIcosahedron icosahedron;

	GameObject model;

	Mesh stand;
	Transform modelStandTransform;
	Transform icosahedronStandTransform;

	std::vector<Mesh> shadowVolumes;

	Material transparentMat;

	float t = 0;
};
