#include "World2.h"

#include "Core/Application.h"


World2::~World2()
{
	if (skybox != nullptr) delete skybox;

	if (mExitPortal != nullptr) delete mExitPortal;
}

void World2::OnSetup()
{
	skybox = new Skybox("gfx/skybox.png");

	directionalLight.setType(Light::LightType::Directional);
	directionalLight.setDiffuseColor(1.0f);
	directionalLight.setAmbientColor(0.15f);
	directionalLight.setSpecularColor(1.0f);
	directionalLight.setPosition({ -1, 1, 0 });

	mExitPortal = new Portal(this);
	mEntryPortal = mExitPortal;

	groundPlane = GeometryHelper::CreatePlane(10, 10);
	model = GeometryHelper::LoadObj("models/bro.obj");

	// move the camera up slightly
	sceneCamera->setPosition({ 0, 1, 6 });
}

void World2::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt, false);
}

void World2::OnUpdate(float dt)
{
	mExitPortal->TestForTravelling(input, sceneCamera);
}

void World2::OnRender()
{
	skybox->render(sceneCamera->getPosition());

	directionalLight.render(GL_LIGHT0);
	Material::Default.apply();


	mExitPortal->Render();

	// render the rest of the scene as normal
	{
		Transformation t({ 0, 0, -3 }, { 0, 90, 0 }, { 0.1f, 0.1f, 0.1f });
		RenderHelper::drawMesh(model);
	}
	{
		Transformation t{ {0, 0, 0}, {0, 0, 0}, {10, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}
}

void World2::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
