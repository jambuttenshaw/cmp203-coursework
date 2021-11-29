#pragma once

#include "Objects/PortalScene.h"
#include "Objects/Portal.h"


class World2 : public PortalScene
{
public:
	World2() = default;
	~World2();

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender() override;

	inline virtual const Skybox* GetSkybox() const override { return skybox; }

	virtual void SetExitPortal(Portal* p) override;
	inline virtual Portal* GetEntryPortal() const override { return mExitPortal; }

private:
	Skybox* skybox = nullptr;
	Light directionalLight;

	Mesh groundPlane;
	Mesh model;
};
