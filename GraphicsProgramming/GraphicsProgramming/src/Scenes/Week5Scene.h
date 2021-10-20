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
	bool escapePressed = false;

	Light sceneLight;
	Light spotLight;

	Texture* quadTexture;
	Texture* groundTexture;

	Material shiny;
	Material defaultMat;

	Mesh plane;
	Mesh cube;
};

