#include "PortalScene.h"

#include "Core/Application.h"


PortalScene::~PortalScene()
{
	if (skybox != nullptr) delete skybox;

	if (portal != nullptr) delete portal;
}

void PortalScene::OnSetup()
{
	skybox = new Skybox("gfx/skybox3.png");

	directionalLight.setType(Light::LightType::Directional);
	directionalLight.setDiffuseColor(1.0f);
	directionalLight.setAmbientColor(0.15f);
	directionalLight.setSpecularColor(1.0f);
	directionalLight.setPosition({ -1, 1, 0 });

	portal = new Portal(this);

	portal->GetTransform().SetTranslation({ 0, 0, 0 });

	portal->SetLinkedPortal(nullptr);

	groundPlane = GeometryHelper::CreatePlane(10, 10);
	sphere = GeometryHelper::CreateUnitSphere(150);

	// move the camera up slightly
	sceneCamera->setPosition({ 0, 1, 6 });

	red.setAmbientAndDiffuse(Color::Red);
	blue.setAmbientAndDiffuse(Color::Blue);
	green.setAmbientAndDiffuse(Color::Green);
	yellow.setAmbientAndDiffuse(Color::Yellow);
}

void PortalScene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt, false);
}

void PortalScene::OnUpdate(float dt)
{
	portal->TestForTravelling(input, sceneCamera->getPosition());
}

void PortalScene::OnRender()
{
	skybox->render(sceneCamera->getPosition());

	directionalLight.render(GL_LIGHT0);
	Material::Default.apply();


	portal->Render();


	// render the rest of the scene as normal
	blue.apply();
	{
		Transformation t{ {2.5f, 0, 0}, {0, 0, 0}, {5, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}
	red.apply();
	{
		Transformation t{ {-2.5f, 0, 0}, {0, 0, 0}, {5, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}

	yellow.apply();
	{
		Transformation t({ 0, 1.0f, 4.0f }, { 0, 0, 0 }, { 0.5f, 0.5f, 0.5f });
		RenderHelper::drawUnitCube();
	}
	green.apply();
	{
		Transformation t({ -1.5f, 1.5f, 5.0f }, { 0, 0, 0 }, { 0.5f, 0.5f, 0.5f });
		RenderHelper::drawUnitCube();
	}
	{
		Transformation t({ 2.0f, 0.8f, -5.0f }, { 30, 45, 0 }, { 0.5f, 0.5f, 0.5f });
		RenderHelper::drawMesh(sphere);
	}
}

void PortalScene::LinkPortalsTo(Portal* p)
{
	portal->SetLinkedPortal(p);
}
