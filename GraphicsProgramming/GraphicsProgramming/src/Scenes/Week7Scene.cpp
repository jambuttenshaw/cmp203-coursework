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
	
	groundTexture = new Texture("gfx/sand.png", Texture::MIPMAPS);
	groundTexture->SetSampleMode(Texture::SampleMode::Repeat);
	groundTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	ground = GeometryHelper::CreatePlane(250, 250, {0, 1, 0}, 50, 50, [](float x, float z)->float {return 3 * sinf(10.0f * x * x) + z; });
	ground.MeshTexture = groundTexture;

	metal.setAmbientAndDiffuse(Color::White);
	metal.setSpecular(Color::White);
	metal.setShininess(128);

	metalTexture = new Texture("gfx/metal.png", Texture::MIPMAPS);
	metalTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	cube = GeometryHelper::CreateUnitCube(5);
	cube.MeshTexture = metalTexture;

	disc = GeometryHelper::CreateDisc(1.0f, 50);
	sphere = GeometryHelper::CreateUnitSphere(4000);
	cylinder = GeometryHelper::CreateCylinder(2, 1, 75);
	disc.MeshTexture = metalTexture;
	sphere.MeshTexture = metalTexture;
	cylinder.MeshTexture = metalTexture;

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
		Transformation t({0, 0, 0}, {0, 0, 0}, { 30, 1, 30 });
		RenderHelper::drawMesh(ground);
	}

	metal.apply();
	{
		Transformation t({ -3, 4, 0 });
		RenderHelper::drawMesh(disc);
	}
	{
		Transformation t({ 0, 4, 0 });
		RenderHelper::drawMesh(cylinder);
	}
	{
		Transformation t({ 3, 4, 0 });
		RenderHelper::drawMeshWireframeOverlay(sphere);
	}
}
