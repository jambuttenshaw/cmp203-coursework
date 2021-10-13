#pragma once

#include "Core/Scene.h"
#include "Core/Color.h"

#include "Rendering/Light.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

class TestScene : public Scene
{
public:
	TestScene() = default;

	// Inherited via Scene
	virtual void OnSetup() override;
	virtual void OnHandleInput(float dt) override;
	virtual void OnUpdate(float dt) override;
	virtual void OnPositionCamera() override;
	virtual void OnRender() override;

private:
	Color globalAmbience = Color::Black;

	Mesh plane;

	Light redLight;
	Light blueLight;

	Material planeMat;	
	Material metallic;
};
