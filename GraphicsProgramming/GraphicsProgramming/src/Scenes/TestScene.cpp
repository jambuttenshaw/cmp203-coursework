#include "TestScene.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Rendering/Transform.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/GeometryHelper.h"

void TestScene::OnSetup()
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbience.ptr());

	light.setType(Light::LightType::Point);
	light.setPosition({ 0.0f, 0.0f, 0.0f });
	light.setDiffuseColor(Color::White);
	light.setSpecularColor(Color::White);
	light.setAmbientColor({ 0.3f, 0.3f, 0.3f });
	light.setAttenuation({ 1.0f, 0.0f, 0.0f });


	plane = GeometryHelper::CreatePlane(100, 100, GeometryHelper::HeightFuncs::Flat);


	// create a default material
	mat.setAmbientAndDiffuse({ 0.35f, 0.35f, 0, 1 });
	mat.setSpecular(0.5f);


	metallic.setAmbientAndDiffuse({ 0.8f });
	metallic.setSpecular(Color::White);
	metallic.setShininess(128.0f);
}

void TestScene::OnHandleInput(float dt)
{
	if (input->isKeyDown('w')) lightY += 0.3f * dt;
	if (input->isKeyDown('s')) lightY -= 0.3f * dt;


	if (input->isKeyDown('e')) shiny = std::min(128.0f, shiny + 30 * dt);
	if (input->isKeyDown('q')) shiny = std::max(0.0f, shiny - 30 * dt);
	mat.setShininess(shiny);
	mat.setSpecular(shiny / 128.0f);
}

void TestScene::OnUpdate(float dt)
{
}

void TestScene::OnPositionCamera()
{
	gluLookAt(0.0f, 3.0f, 9.0f, 0.0f, 2.5f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void TestScene::OnRender()
{

	{
		Transform t({ 0, lightY, 2.5f }, { 0, 0, 0 }, { 1, 1, 1 });
		light.render(GL_LIGHT0);
	}

	mat.apply();
	{
		Transform t({ 0, 0, 0 }, { 0, 0, 0 }, { 5, 1, 5 });
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ 0, 2.5f, -2.5f }, { 90, 0, 0 }, { 5, 1, 5 });
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ -2.5f, 2.5f, 0 }, { 0, 0, -90 }, { 5, 1, 5 });
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ 2.5f, 2.5f, 0 }, { 0, 0, 90 }, { 5, 1, 5 });
		RenderHelper::drawMesh(plane);
	}
	metallic.apply();
	{
		Transform t({0, 0.5f, 0}, Vector3::zero, Vector3::one);
		RenderHelper::drawSphere(0.5f);
	}

}
