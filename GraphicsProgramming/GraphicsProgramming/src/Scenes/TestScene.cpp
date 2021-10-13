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


	plane = GeometryHelper::CreatePlane(20, 20, GeometryHelper::HeightFuncs::Flat);


	// create a default material
	planeMat.setAmbientAndDiffuse(0.9f);
	planeMat.setSpecular(0.2f);
	planeMat.setShininess(5);


	metallic.setAmbientAndDiffuse({ 0.8f });
	metallic.setSpecular(Color::White);
	metallic.setShininess(128.0f);
		
	Application::SetCursorDisabled(true);
	sceneCamera->setPosition({ 0, 3, 6 });
}

void TestScene::OnHandleInput(float dt)
{
	sceneCamera->Process3DControllerInputs(dt);
}

void TestScene::OnUpdate(float dt)
{
}

void TestScene::OnRender()
{

	{
		Transform t({ -2, 3, -2 }, Vector3::zero, Vector3::one);
		redLight.render(GL_LIGHT0);
	}
	{
		Transform t({ 2, 3, -2 }, Vector3::zero, Vector3::one);
		blueLight.render(GL_LIGHT1);
	}

	Vector3 planeScale{ 5, 1, 5 };

	planeMat.apply();
	{
		Transform t(Vector3::zero, Vector3::zero, planeScale);
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ 0, 2.5f, -2.5f }, { 90, 0, 0 }, planeScale);
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ -2.5f, 2.5f, 0 }, { 0, 0, -90 }, planeScale);
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ 2.5f, 2.5f, 0 }, { 0, 0, 90 }, planeScale);
		RenderHelper::drawMesh(plane);
	}


	metallic.apply();
	{
		Transform t({0, 0.5f, 0}, Vector3::zero, Vector3::one);
		RenderHelper::drawSphere(0.5f, 30, 30);
	}
	{
		Transform t({ 0, 0.5f, 10 }, Vector3::zero, Vector3::one);
		RenderHelper::drawSphere(0.5f, 30, 30);
	}

}
