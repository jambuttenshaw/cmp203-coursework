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

	enableShadowVolumes(true);

	pointLight.setType(Light::LightType::Point);
	pointLight.setDiffuseColor(1.0f);
	pointLight.setAmbientColor(0.15f);
	pointLight.setSpecularColor(1.0f);
	pointLight.setPosition({ 5, 4, -8 });
	RegisterLight(&pointLight);

	mExitPortal = new Portal(this);
	mEntryPortal = mExitPortal;

	groundPlane = GeometryHelper::CreatePlane(10, 10);

	model = GeometryHelper::LoadObj("models/bro.obj");
	modelTransform.SetTranslation({ 0, 0, -3 });
	modelTransform.SetRotation({ 0, 90, 0 });
	modelTransform.SetScale({ 0.1f, 0.1f, 0.1f });
	modelShadowVolume = ShadowHelper::BuildShadowVolume(model, modelTransform.LocalToWorld(), pointLight.getPosition());

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
	sceneCamera->Process3DControllerInputs(dt, true);
}

void World2::OnUpdate(float dt)
{
	mExitPortal->TestForTravelling(input, sceneCamera);
}

void World2::OnRenderObjects()
{
	Material::Default.apply();


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

void World2::OnRenderShadowVolumes()
{
	RenderHelper::drawMesh(modelShadowVolume);
}

void World2::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
