#pragma once

#include "Core/Scene.h"

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

	Mesh portal;
	Mesh portalScreen;
	Mesh groundPlane;

	Material red;

	float rot = 0.0f;
};
