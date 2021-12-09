#pragma once

#include "Objects/PortalScene.h"
#include "Objects/Portal.h"


class World1 : public PortalScene
{
public:
	World1() = default;
	~World1();

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

	GameObject portalPlatform;
	GameObject basePlatform;
	GameObject longPlatform;

	// robot arm
	GameObject armBase;
	GameObject armMiddle;
	GameObject armTop;


	Material portalPlatformMat;


	float baseRotation = 0.0f;
	float t = 0.0f;
	float t2 = 0.0f;
};
