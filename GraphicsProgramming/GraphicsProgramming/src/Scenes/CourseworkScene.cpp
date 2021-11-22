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
	glDepthMask(GL_FALSE);
	// enable & setup stencil test
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	// draw our stencil
	RenderHelper::drawMesh(portalScreen);

	// re-enable writing to frame buffer
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// we do not want to change the stencil buffer from here
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


	// render everything on one side of the portal
	glStencilFunc(GL_EQUAL, 1, 1);

	skybox2->render(sceneCamera->getPosition());
	{
		Transformation t({ 0, 0.5f, -3.0f }, { 0.5f * rot, 1.2f * rot, 0.0f }, Vector3::one);
		RenderHelper::drawUnitCube();
	}
	

	// render the rest of the skybox, apart from the area inside the portal
	glStencilFunc(GL_GREATER, 1, 1);
	skybox->render(sceneCamera->getPosition());

	// now we want to fill in the depth information of the portal, since everything behind it has already been drawn
	glDisable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	RenderHelper::drawMesh(portalScreen);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_STENCIL_TEST);

	// render everything on the other side of the portal
	{
		Transformation t{ Vector3::zero, Vector3::zero, {10, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}
	{
		Transformation t({ 0, 1.0f, -3.0f });
		RenderHelper::drawSphere(1.0f);
	}

	// render everything on both sides of the portal
	RenderHelper::drawMesh(portal);
}
