#pragma once

#include "Objects/PortalScene.h"
#include "Objects/Portal.h"

#include "Objects/Spotlight.h"

#include <array>


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
	Light dirLight;

	GameObject portalPlatform;
	GameObject basePlatform;
	GameObject longPlatform;

	// robot arm
	GameObject armBase;
	GameObject armMiddle;
	GameObject armTop;

	std::array<Spotlight, 4> spotlights;


	Material portalPlatformMat;
	Material darkMat;


	float baseRotation = 0.0f;
	float t = 1.3f;
	float t2 = 2.7f;
};
