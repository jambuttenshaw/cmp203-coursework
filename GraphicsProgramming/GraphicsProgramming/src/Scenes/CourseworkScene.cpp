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


	{
		Transformation t({ -2, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
		portal->Render();
	}
	{
		Transformation t({ 2, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
		portal2->Render();
	} 


	// render the rest of the scene as normal
	{
		Transformation t{ {0, 0, 0}, {0, 0, 0}, {10, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}
	{
		Transformation t({ 0, 1.0f, 3.0f }, {0, 0, 0}, {0.5f, 0.5f, 0.5f});
		RenderHelper::drawMesh(sphere);
	}
}
