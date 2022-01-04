#pragma once

#include "Objects/PortalScene.h"
#include "Objects/Portal.h"

#include "Objects/Spotlight.h"
#include "Objects/Billboard.h"

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
	// the main directional light in the scene, that originates from the sun
	Light dirLight;
	
	// all textures used in the scene
	Texture* starTexture = nullptr;
	Texture* spaceshipTexture = nullptr;
	Texture* marbleTexture = nullptr;
	Texture* sunTexture = nullptr;

	// the platforms
	GameObject portalPlatform;
	GameObject basePlatform;
	GameObject longPlatform;

	// robot arm
	GameObject armBase;
	GameObject armMiddle;
	GameObject armTop;

	// the other objects in the scene
	GameObject spaceship;
	GameObject sun;

	// a bunch of spotlights to illuminate the robot arm and the dome
	std::array<Spotlight, 4> spotlights;

	// the big dome to make the light from the spotlights more obvious
	TransparentObject dome;

	// some billboard objets using a star texture
	std::array<Billboard*, 6> stars{ nullptr };

	// all the materials used in the scene
	Material domeMat;
	Material portalPlatformMat;
	Material armMat;
	Material spaceshipMat;
	Material sunMat;

	// all shadow volumes in the scene
	std::vector<Mesh> shadowVolumes;

	// a few parameters to control things happening in the scene
	float baseRotation = 0.0f;
	float t = 1.3f;
	float t2 = 2.7f;

	float spotlightColorParam = 0.0f;
};
