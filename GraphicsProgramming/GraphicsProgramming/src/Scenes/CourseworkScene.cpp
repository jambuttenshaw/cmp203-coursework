#include "CourseworkScene.h"

#include "Core/Application.h"


CourseworkScene::~CourseworkScene()
{
	if (skybox != nullptr) delete skybox;
	if (skybox2 != nullptr) delete skybox2;
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

	portal = GeometryHelper::LoadObj("models/portal.obj");
	portalScreen = GeometryHelper::LoadObj("models/portalScreen.obj");
	groundPlane = GeometryHelper::CreatePlane(10, 10);

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
	directionalLight.render(GL_LIGHT0);
	Material::Default.apply();


	// draw portal screen into stencil buffer

	// disable writing to frame buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	// enable & setup stencil test
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// when drawing into stencil buffer we do not want to write to depth buffer
	glDisable(GL_DEPTH_TEST);
	// draw our stencil
	RenderHelper::drawMesh(portalScreen);

	// re-enable depth test
	glEnable(GL_DEPTH_TEST);
	// re-enable writing to frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// now we want to write to the framebuffer only when the stencil test is 1 (as we specified that the stencil buffer will have 1's written to it on line 53)
	glStencilFunc(GL_EQUAL, 1, 1);
	// we do not want to change the stencil buffer from here
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// render all objects we want to be rendered under the stencil test
	skybox2->render(sceneCamera->getPosition());
	{
		Transformation t({ 0, 0.5f, -3.0f }, { 0.5f * rot, 1.2f * rot, 0.0f }, Vector3::one);
		RenderHelper::drawUnitCube();
	}

	glStencilFunc(GL_GREATER, 1, 1);

	skybox->render(sceneCamera->getPosition());

	{
		Transformation t{ Vector3::zero, Vector3::zero, {10, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}
	RenderHelper::drawMesh(portal);

	glDisable(GL_STENCIL_TEST);
}
