#pragma once

#include "Objects/PortalScene.h"
#include "Objects/Portal.h"

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

	Mesh groundPlane;
	
	Mesh sphere;
	Transform sphereTransform;
	Mesh sphereShadowVolume;

	Material red;
	Material blue;
	Material green;
	Material yellow;

	float temp = 0;
};
