#pragma once

#include "Core/Scene.h"

class TestScene : public Scene
{
public:
	TestScene() = default;
	~TestScene();

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnRender() override;

private:
	Skybox* skybox;

	Light light;
	float* shadowMatrix;

	Material defaultMat;
	Mesh plane;
};
