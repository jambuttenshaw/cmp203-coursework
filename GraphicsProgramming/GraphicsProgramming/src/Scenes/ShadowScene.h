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
	virtual void OnRenderShadowVolumes() override;
	virtual void OnRenderObjects() override;

private:
	Texture* groundTex = nullptr;
	Texture* modelTex = nullptr;

	Light pointLight;

	Mesh groundPlane;

	Transform modelTransform;
	Mesh model;

	Mesh shadowVolume;

	float t = 0;
};

