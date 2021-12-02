#include "CourseworkScene.h"

#include "Core/Application.h"


CourseworkScene::~CourseworkScene()
{
	if (skybox != nullptr) delete skybox;

	if (mExitPortal != nullptr) delete mExitPortal;
}

void CourseworkScene::OnSetup()
{
	skybox = new Skybox("gfx/skybox2.png");

	enableShadowVolumes(true);

	pointLight.setType(Light::LightType::Point);
	pointLight.setDiffuseColor(1.0f);
	pointLight.setAmbientColor(0.15f);
	pointLight.setSpecularColor(1.0f);
	pointLight.setPosition({ -4, 4, 0 });
	RegisterLight(&pointLight);

	mExitPortal = new Portal(this);
	// this scene is to be entered and exited by the same portal
	mEntryPortal = mExitPortal;

	groundPlane = GeometryHelper::CreatePlane(10, 10);
	sphere = GeometryHelper::CreateUnitSphere(150);

	// move the camera up slightly
	sceneCamera->setPosition({ 1.5f, 1.0f, 3.0f });

	red.setAmbientAndDiffuse(Color::Red);
	blue.setAmbientAndDiffuse(Color::Blue);
	green.setAmbientAndDiffuse(Color::Green);
	yellow.setAmbientAndDiffuse(Color::Yellow);
}

void CourseworkScene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt, false);
}

void CourseworkScene::OnUpdate(float dt)
{
	mExitPortal->TestForTravelling(input, sceneCamera);
}

void CourseworkScene::OnRenderObjects()
{
	Material::Default.apply();


	mExitPortal->Render();

	// render the rest of the scene as normal
	green.apply();
	{
		Transformation t{ {2.5f, 0, 0}, {0, 0, 0}, {5, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}
	yellow.apply();
	{
		Transformation t{ {-2.5f, 0, 0}, {0, 0, 0}, {5, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}

	red.apply();
	{
		Transformation t({ 0, 1.0f, 4.0f }, {0, 0, 0}, {0.5f, 0.5f, 0.5f});
		RenderHelper::drawMesh(sphere);
	}
	blue.apply();
	{
		Transformation t({ -1.5f, 1.5f, 5.0f }, { 0, 0, 0 }, { 0.5f, 0.5f, 0.5f });
		RenderHelper::drawMesh(sphere);
	}
	{
		Transformation t({ 2.0f, 0.8f, -5.0f }, { 30, 45, 0 }, { 0.5f, 0.5f, 0.5f });
		RenderHelper::drawUnitCube();
	}
}

void CourseworkScene::OnRenderShadowVolumes()
{
}

void CourseworkScene::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
