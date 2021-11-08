#include "Week7Scene.h"

#include "Core/Application.h"

Week7Scene::~Week7Scene()
{
	if (skybox != nullptr)
		delete skybox;	

	if (groundTexture != nullptr)
		delete groundTexture;

	if (metalTexture != nullptr)
		delete metalTexture;
}

void Week7Scene::OnSetup()
{
	dirLight.setType(Light::LightType::Directional);
	dirLight.setDiffuseColor(1.0f);
	dirLight.setAmbientColor(0.15f);
	dirLight.setSpecularColor(1.0f);
	dirLight.setPosition({ -1, 1, 0 });

	// load skybox
	skybox = new Skybox("gfx/skybox2.png");
	
	groundTexture = new Texture("gfx/sand.png", true);
	groundTexture->SetSampleMode(Texture::SampleMode::Repeat);
	groundTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	ground = GeometryHelper::CreatePlane(250, 250, Vector3::up, 50, 50, GeometryHelper::HeightFuncs::PerlinNoiseTerrain);
	ground.Texture = groundTexture;

	metal.setAmbientAndDiffuse(Color::White);
	metal.setSpecular(Color::White);
	metal.setShininess(128);

	metalTexture = new Texture("gfx/metal.png", true);
	metalTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	cube = GeometryHelper::CreateUnitCube(50);
	cube.Texture = metalTexture;

	Application::SetCursorDisabled(true);
}

void Week7Scene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}

	sceneCamera->Process3DControllerInputs(dt);
}

void Week7Scene::OnUpdate(float dt)
{
	rot += 40 * dt;
}

void Week7Scene::OnRender()
{
	skybox->render(sceneCamera->getPosition());

	dirLight.render(GL_LIGHT0);

	defaultMat.apply();
	{
		Transform t(Vector3::zero, Vector3::zero, { 30, 1, 30 });
		groundTexture->Bind();
		RenderHelper::drawMesh(ground);
	}

	metal.apply();
	metalTexture->Bind();
	{
		Transform t({0, 1, 0}, { 0, rot, 0 }, Vector3::one);
		RenderHelper::drawMesh(cube);
	}
	{
		Transform t({ 2, 1, 0 }, { 0, rot, 0 }, Vector3::one);
		RenderHelper::drawMesh(cube);
	}
	{
		Transform t({ 0, 1, 2 }, { 0, rot, 0 }, Vector3::one);
		RenderHelper::drawMesh(cube);
	}
	{
		Transform t({ 2, 1, 2 }, { 0, rot, 0 }, Vector3::one);
		RenderHelper::drawMesh(cube);
	}
	{
		Transform t({ -2, 1, -2 }, { 0, rot, 0 }, Vector3::one);
		RenderHelper::drawMesh(cube);
	}
}
