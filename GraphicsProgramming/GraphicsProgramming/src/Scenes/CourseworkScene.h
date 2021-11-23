#pragma once

#include "Core/Scene.h"
#include "Objects/Portal.h"

class CourseworkScene : public Scene
{
public:
	CourseworkScene() = default;
	~CourseworkScene();

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender() override;

private:
	Skybox* skybox;
	Skybox* skybox2;
	Light directionalLight;

	Portal* portal;
	Portal* portal2;

	Mesh groundPlane;
	Mesh sphere;

	Material red;

	float rot = 0.0f;
};
