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

	enableShadowVolumes(true);

	dirLight.setType(Light::LightType::Directional);
	dirLight.setDiffuseColor({ 0.9f, 0.9f, 0.7f });
	dirLight.setAmbientColor(0.15f);
	dirLight.setSpecularColor(1.0f);
	dirLight.setPosition({ -25, 87, 43 });
	RegisterLight(&dirLight);

	mExitPortal = new Portal(this);
	mEntryPortal = mExitPortal;
	mExitPortal->GetTransform().SetTranslation({ 0, 0, -5 });


	sandTexture = new Texture("gfx/sand.png", Texture::Flags::MIPMAPS);
	sandTexture->SetSampleMode(Texture::SampleMode::Repeat);
	sandTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	windowTexture = new Texture("gfx/window.png", Texture::Flags::MIPMAPS);
	windowTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);


	groundPlane = GeometryHelper::CreatePlane(200, 200, { 0, 1, 0 }, 10, 10, GeometryHelper::HeightFuncs::PerlinNoiseTerrain);
	groundPlane.MeshTexture = sandTexture;

	model.GetMesh() = GeometryHelper::LoadObj("models/bro.obj");
	model.GetTransform().SetTranslation({ -2, 0, -1 });
	model.GetTransform().SetScale({ 0.1f, 0.1f, 0.1f });


	modelShadowVolume = ShadowHelper::BuildShadowVolume(model, dirLight.getPosition());


	transparentMat.setAmbientAndDiffuse({ 1.0f, 0.0f, 0.0f, 0.5f });

	// move the camera up slightly
	sceneCamera->setPosition({ 0, 1, 6 });


	// set up rendering transparent objects


	// windows
	window1.mat = &Material::Default;
	window1.gameObject.GetTransform().SetTranslation({ 2, 2, 2 });
	window1.gameObject.GetTransform().SetRotation({ 90, 0, -45 });
	window1.gameObject.GetTransform().SetScale(glm::vec3(2.0f));
	window1.renderObject = [this]() {
		window1.mat->apply();
		Transformation t(window1.gameObject);
		RenderHelper::drawQuad(windowTexture);
	};
	RegisterTransparentObject(&window1);

	window2.mat = &Material::Default;
	window2.gameObject.GetTransform().SetTranslation({ 4, 2, 4 });
	window2.gameObject.GetTransform().SetRotation({ 90, 0, -45 });
	window2.gameObject.GetTransform().SetScale(glm::vec3(2.0f));
	window2.renderObject = [this]() {
		window2.mat->apply();
		Transformation t(window2.gameObject);
		RenderHelper::drawQuad(windowTexture);
	};
	RegisterTransparentObject(&window2);

	icosahedron.mat = &Material::Default;
	icosahedron.gameObject.GetTransform().SetTranslation({ 3, 3, 3 });
	icosahedron.renderObject = [this]() {
		icosahedron.mat->apply();
		Transformation t(icosahedron.gameObject);
		icosahedron.Render(GetActiveCamera()->getPosition());
	};
	RegisterTransparentObject(&icosahedron);
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
	icosahedron.gameObject.GetTransform().SetTranslation({ 3, 3 + sinf(t), 3 });
}

void World2::OnRenderObjects()
{
	Material::Default.apply();


	// render the rest of the scene as normal
	{
		Transformation t(model);
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
}


void World2::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
