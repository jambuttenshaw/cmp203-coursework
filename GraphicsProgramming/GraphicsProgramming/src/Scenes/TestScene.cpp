#include "TestScene.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Core/Application.h"

#include "Rendering/Transform.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/GeometryHelper.h"

void TestScene::OnSetup()
{
	setGlobalAmbientLighting(globalAmbience);

	redLight.setType(Light::LightType::Point);
	redLight.setPosition(Vector3::zero);
	redLight.setDiffuseColor(Color::Red);
	redLight.setSpecularColor(0.7f);
	redLight.setAmbientColor(0.3f);
	redLight.setAttenuation({ 1.0f, 0.125f, 0.05f });

	blueLight.setType(Light::LightType::Point);
	blueLight.setPosition(Vector3::zero);
	blueLight.setDiffuseColor(Color::Blue);
	blueLight.setSpecularColor(0.7f);
	blueLight.setAmbientColor(0.3f);
	blueLight.setAttenuation({ 1.0f, 0.125f, 0.05f });

	whiteLight.setType(Light::LightType::Point);
	whiteLight.setPosition(Vector3::zero);
	whiteLight.setDiffuseColor(0.8f);
	whiteLight.setSpecularColor(0.85f);
	whiteLight.setAmbientColor(0.2f);
	whiteLight.setAttenuation({ 1.0f, 0.12f, 0.01f });


	plane = GeometryHelper::CreatePlane(20, 20);


	// create a default material
	planeMat.setAmbientAndDiffuse(0.9f);
	planeMat.setSpecular(0.2f);
	planeMat.setShininess(5);


	metallic.setAmbientAndDiffuse({ 0.8f });
	metallic.setSpecular(Color::White);
	metallic.setShininess(128.0f);
		
	Application::SetCursorDisabled(true);
	sceneCamera->setPosition({ 0, 3, 2.5f });
}

void TestScene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE) && !escapePressed)
	{
		escapePressed = true;
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
	}
	if (!input->isKeyDown(VK_ESCAPE) && escapePressed)
	{
		escapePressed = false;
	}
	sceneCamera->Process3DControllerInputs(dt);
}

void TestScene::OnRender()
{

	{
		Transform t({ -4, 3, -4 }, Vector3::zero, Vector3::one);
		redLight.render(GL_LIGHT0);
	}
	{
		Transform t({ 4, 3, 0 }, Vector3::zero, Vector3::one);
		blueLight.render(GL_LIGHT1);
	}
	{
		Transform t({ 0, 8, 0 }, Vector3::zero, Vector3::one);
		whiteLight.render(GL_LIGHT3);
	}

	float planeScaleEdge = 10.0f;
	Vector3 planeScale{ planeScaleEdge, 1, planeScaleEdge };

	planeMat.apply();
	{
		Transform t(Vector3::zero, Vector3::zero, planeScale);
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ 0, 0.5f * planeScaleEdge, -0.5f * planeScaleEdge }, { 90, 0, 0 }, planeScale);
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ -0.5f * planeScaleEdge, 0.5f * planeScaleEdge, 0 }, { 0, 0, -90 }, planeScale);
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ 0.5f * planeScaleEdge, 0.5f * planeScaleEdge, 0 }, { 0, 0, 90 }, planeScale);
		RenderHelper::drawMesh(plane);
	}


	metallic.apply();
	{
		Transform t({3.0f, 1.5f, -3.0f}, Vector3::zero, Vector3::one);
		RenderHelper::drawSphere(1.5f, 30, 30);
	}
	{
		Transform t({ -3.0f, 1, 3.0f }, Vector3::zero, Vector3::one);
		RenderHelper::drawSphere(1, 30, 30);
	}
}
