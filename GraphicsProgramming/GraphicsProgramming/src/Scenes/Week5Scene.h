#pragma once

#include "Core/Scene.h"


class Week5Scene : public Scene
{
public:
	Week5Scene() = default;
	~Week5Scene();

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender() override;

private:
	Light sceneLight;
	Light spotLight;

	Texture* quadTexture;
	Texture* groundTexture;
	Texture* metalTexture;

	Material defaultMat;
	Material metalMat;

	Mesh plane;
	Mesh cube;


	// gameplay
	bool spotOn = false;
};

