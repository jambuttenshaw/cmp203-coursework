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

	Mesh ground;
};
