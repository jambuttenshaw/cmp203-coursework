#include "World2.h"

#include "Core/Application.h"


World2::~World2()
{
	if (skybox != nullptr) delete skybox;
	if (mExitPortal != nullptr) delete mExitPortal;
	if (sandTexture != nullptr) delete sandTexture;
	if (windowTexture != nullptr) delete windowTexture;
}

void World2::OnSetup()
{
	skybox = new Skybox("gfx/skybox2.png");

	enableShadowVolumes(false);

	dirLight.setType(Light::LightType::Directional);
	dirLight.setDiffuseColor({ 0.9f, 0.9f, 0.7f });
	dirLight.setAmbientColor(0.15f);
	dirLight.setSpecularColor(1.0f);
	dirLight.setPosition({ -25, 87, 43 });
	RegisterLight(&dirLight);

	mExitPortal = new Portal(this);
	mEntryPortal = mExitPortal;


	sandTexture = new Texture("gfx/sand.png", Texture::Flags::MIPMAPS);
	sandTexture->SetSampleMode(Texture::SampleMode::Repeat);
	sandTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	windowTexture = new Texture("gfx/window.png", Texture::Flags::MIPMAPS);
	windowTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);


	groundPlane = GeometryHelper::CreatePlane(200, 200, { 0, 1, 0 }, 10, 10, GeometryHelper::HeightFuncs::PerlinNoiseTerrain);
	groundPlane.MeshTexture = sandTexture;

	proceduralSphere.gameObject.GetMesh() = GeometryHelper::CreateUnitSphere(100);
	proceduralSphere.gameObject.GetTransform().SetTranslation({ -4, 1.5f, 1 });
	proceduralSphere.gameObject.GetTransform().SetScale(glm::vec3{ 0.25f });
	

	model = GeometryHelper::LoadObj("models/bro.obj");
	modelTransform.SetTranslation({ 0, 0, -3 });
	modelTransform.SetRotation({ 0, 90, 0 });
	modelTransform.SetScale({ 0.1f, 0.1f, 0.1f });


	sphereShadowVolume = ShadowHelper::BuildShadowVolume(proceduralSphere.gameObject, dirLight.getPosition());
	modelShadowVolume = ShadowHelper::BuildShadowVolume(model, modelTransform.LocalToWorld(), dirLight.getPosition());


	transparentMat.setAmbientAndDiffuse({ 1.0f, 0.0f, 0.0f, 0.5f });

	// move the camera up slightly
	sceneCamera->setPosition({ 0, 1, 6 });


	// set up rendering transparent objects
	proceduralSphere.mat = &transparentMat;
	proceduralSphere.renderObject = [this]() {
		proceduralSphere.mat->apply();
		Transformation t(proceduralSphere.gameObject);
		RenderHelper::drawMesh(proceduralSphere.gameObject);
	};
	RegisterTransparentObject(&proceduralSphere);


	window.mat = &Material::Default;
	window.renderObject = [this]() {
		window.mat->apply();
		Transformation t({ 0, 2, 2 }, { 90, 0, 0 }, { 2, 2, 2 });
		RenderHelper::drawQuad(windowTexture);
	};
	RegisterTransparentObject(&window);
}

void World2::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt, true);
}

void World2::OnUpdate(float dt)
{
	mExitPortal->TestForTravelling(input, sceneCamera);


	t += 0.5f * dt;
	proceduralSphere.gameObject.GetTransform().SetTranslation({ -4, 1.5f + sinf(t), cosf(t) });
	sphereShadowVolume = ShadowHelper::BuildShadowVolume(proceduralSphere.gameObject, dirLight.getPosition());

}

void World2::OnRenderObjects()
{
	Material::Default.apply();


	// render the rest of the scene as normal
	{
		Transformation t({ 0, 0, -3 }, { 0, 90, 0 }, { 0.1f, 0.1f, 0.1f });
		RenderHelper::drawMesh(model);
	}
	{
		Transformation t{ {0, 0, 0}, {0, 0, 0}, {50, 1, 50} };
		RenderHelper::drawMesh(groundPlane);
	}
	

	RenderTransparentObjects();
}

void World2::OnRenderShadowVolumes()
{
	RenderHelper::drawMesh(modelShadowVolume);
	RenderHelper::drawMesh(sphereShadowVolume);
}

void World2::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
