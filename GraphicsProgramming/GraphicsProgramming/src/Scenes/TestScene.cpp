#include "TestScene.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Core/Application.h"

#include "Rendering/Transformation.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/GeometryHelper.h"
#include "Rendering/Shadow.h"

TestScene::~TestScene()
{
	if (skybox != nullptr)
		delete skybox;

	delete[] shadowMatrix;
}

void TestScene::OnSetup()
{
	setGlobalAmbientLighting(Color::Black);

	skybox = new Skybox("gfx/skybox.png");

	light.setType(Light::LightType::Point);
	light.setAmbientColor(0.2f);
	light.setDiffuseColor(0.7f);
	light.setPosition({ -1, 2, 0 });

	plane = GeometryHelper::CreatePlane(50, 50);
		
	sceneCamera->translate({ 0, 1, 0 });
	Application::SetCursorDisabled(true);

	shadowMatrix = new float[16];

	Shadow::generateShadowMatrix(shadowMatrix, light.getPosition(), plane.Vertices[0].Normal, plane.Vertices[0].Position);
}

void TestScene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt);
}

void TestScene::OnRender()
{
	skybox->render(sceneCamera->getPosition());

	light.render(GL_LIGHT0);

	defaultMat.apply();
	{
		Transformation t(Vector3::zero, Vector3::zero, { 10, 1, 10 });
		//RenderHelper::drawMesh(plane);
	}

	{
		Transformation t({ 0, 0.5f, 0 });
		//RenderHelper::drawUnitCube();
	}

	{
		Transformation t;
		glLoadMatrixf(shadowMatrix);
		RenderHelper::drawUnitCube();
	}
}
