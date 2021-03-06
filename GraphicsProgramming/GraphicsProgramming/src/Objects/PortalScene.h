#pragma once

#include "Core/Scene.h"

class Portal;
class Skybox;

class PortalScene : public Scene
{
public:
	virtual ~PortalScene() {}

	// implement the OnRenderPortals function from Scene,
	// this is the PortalScene's handle into the rendering pipeline
	virtual void OnRenderPortals() override;

	// the portal you will travel from this scene into
	virtual void SetExitPortal(Portal* portal) = 0;
	// the portal you will enter this scene via
	virtual Portal* GetEntryPortal() const = 0;

protected:

	Portal* mEntryPortal = nullptr;
	Portal* mExitPortal = nullptr;
};
