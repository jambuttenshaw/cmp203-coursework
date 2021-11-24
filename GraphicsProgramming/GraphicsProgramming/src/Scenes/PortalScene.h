#pragma once

#include "Core/Scene.h"
#include "Objects/Portal.h"

class PortalScene : public Scene
{
public:
	PortalScene() = default;
	~PortalScene();

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender() override;

	virtual const Skybox* GetSkybox() const override { return skybox; }

	void LinkPortalsTo(Portal* p);
	Portal* GetScenePortal() { return portal; }

private:
	Skybox* skybox = nullptr;
	Light directionalLight;

	Portal* portal = nullptr;

	Mesh groundPlane;
	Mesh sphere;

	Material red;
	Material blue;
	Material green;
	Material yellow;
};
