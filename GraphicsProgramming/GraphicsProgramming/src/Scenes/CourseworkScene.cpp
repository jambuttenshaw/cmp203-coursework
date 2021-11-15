#include "CourseworkScene.h"

#include "Core/Application.h"


CourseworkScene::~CourseworkScene()
{
	if (skybox != nullptr) delete skybox;
}

void CourseworkScene::OnSetup()
{
	skybox = new Skybox("gfx/skybox3.png");

	ground = GeometryHelper::CreatePlane(30, 30);
}

void CourseworkScene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt);
}

void CourseworkScene::OnUpdate(float dt)
{
}

void CourseworkScene::OnRender()
{
	skybox->render(sceneCamera->getPosition());

	Material::Default.apply();
	{
		Transformation t(Vector3::zero, Vector3::zero, { 30, 1, 30 });
		RenderHelper::drawMesh(ground);
	}
}
