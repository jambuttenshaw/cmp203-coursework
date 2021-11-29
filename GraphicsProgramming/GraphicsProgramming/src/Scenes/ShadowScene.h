#pragma once

#include "Core/Scene.h"


class ShadowScene : public Scene
{
public:
	ShadowScene() = default;
	~ShadowScene();

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender() override;


	void RenderSceneObjects();

private:
	Skybox* skybox = nullptr;

	Texture* groundTex = nullptr;
	Texture* modelTex = nullptr;

	const glm::vec3 lightPos = { -3, 4, 0 };
	Light pointLight;

	Mesh groundPlane;
	Mesh model;

	Mesh shadowVolume;
};

