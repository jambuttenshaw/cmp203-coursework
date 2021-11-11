#pragma once

#include "Core/Scene.h"
#include "Rendering/Skybox.h"

class Week7Scene : public Scene
{
public:
	Week7Scene() = default;
	~Week7Scene();

protected:
	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnRender() override;


private:
	Material defaultMat;

	Mesh ground;
	Texture* groundTexture = nullptr;

	Mesh cube;
	Material metal;
	Texture* metalTexture = nullptr;

	Mesh sphere;

	Light dirLight;
	Skybox* skybox = nullptr;

	float rot = 0;
};

