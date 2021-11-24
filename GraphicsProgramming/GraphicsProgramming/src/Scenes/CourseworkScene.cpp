#include "CourseworkScene.h"

#include "Core/Application.h"


CourseworkScene::~CourseworkScene()
{
	if (skybox != nullptr) delete skybox;
	if (skybox2 != nullptr) delete skybox2;

	if (portal != nullptr) delete portal;
	if (portal2 != nullptr) delete portal2;
}

void CourseworkScene::OnSetup()
{
	skybox = new Skybox("gfx/skybox2.png");
	skybox2 = new Skybox("gfx/skybox3.png");

	directionalLight.setType(Light::LightType::Directional);
	directionalLight.setDiffuseColor(1.0f);
	directionalLight.setAmbientColor(0.15f);
	directionalLight.setSpecularColor(1.0f);
	directionalLight.setPosition({ -1, 1, 0 });

	portal = new Portal(this);
	portal2 = new Portal(this);

	portal->GetTransform().SetTranslation({ -2, 0, 0 });
	portal2->GetTransform().SetTranslation({ 2, 0, 0 });

	portal->SetLinkedPortal(portal2);
	portal2->SetLinkedPortal(portal);

	groundPlane = GeometryHelper::CreatePlane(10, 10);
	sphere = GeometryHelper::CreateUnitSphere(150);

	// move the camera up slightly
	sceneCamera->setPosition({ 0, 1, 6 });

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
	rot += 10.0f * dt;
}

void CourseworkScene::OnRender()
{
	skybox->render(sceneCamera->getPosition());

	directionalLight.render(GL_LIGHT0);
	Material::Default.apply();


	portal->Render();
	portal2->Render();


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
