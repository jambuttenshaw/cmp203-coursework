#include "CourseworkScene.h"

#include "Core/Application.h"

#include <cstdlib>


CourseworkScene::~CourseworkScene()
{
	if (mExitPortal != nullptr) delete mExitPortal;
	if (cubeTexture != nullptr) delete cubeTexture;
	if (whiteWallTexture != nullptr) delete whiteWallTexture;
	if (blackWallTexture != nullptr) delete blackWallTexture;
	if (concreteTexture != nullptr) delete concreteTexture;
}

void CourseworkScene::OnSetup()
{
	setGlobalAmbientLighting(0.15f);
	enableShadowVolumes(true);

	// set up lighting
	pointLight.setType(Light::LightType::Point);
	pointLight.setDiffuseColor({0.44f, 0.63f, 0.67f});
	pointLight.setAmbientColor(0.1f);
	pointLight.setSpecularColor(0.7f);
	pointLight.setPosition({ 0, 4.9f, 0 });
	RegisterLight(&pointLight);



	spotLight.setType(Light::LightType::Spot);
	spotLight.setPosition({ 3, 4.9f, 0 });
	spotLight.setDiffuseColor(2.0f);
	spotLight.setSpecularColor(Color::White);
	spotLight.setAmbientColor(0.2f);
	spotLight.setSpotDirection({ 0, -1, 0 });
	spotLight.setSpotExponent(6);
	spotLight.setSpotCutoff(45);
	spotLight.setAttenuation({ 1, 0.2f, 0.05f });
	RegisterLight(&spotLight);


	// set up portals
	mExitPortal = new Portal(this);
	mExitPortal->GetTransform().SetTranslation({ -2.0f, 0.0, -2.5f });
	// this scene is to be entered and exited by the same portal
	mEntryPortal = mExitPortal;


	// load textures
	whiteWallTexture = new Texture("gfx/PortalScene/whiteWall.png", Texture::Flags::MIPMAPS);
	whiteWallTexture->SetSampleMode(Texture::SampleMode::Repeat);
	whiteWallTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	blackWallTexture = new Texture("gfx/PortalScene/blackWall.png", Texture::Flags::MIPMAPS);
	blackWallTexture->SetSampleMode(Texture::SampleMode::Repeat);
	blackWallTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	cubeTexture = new Texture("gfx/portalCube.png");
	cubeTexture->SetFilterMode(Texture::FilterMode::Linear, Texture::FilterMode::Linear);

	concreteTexture = new Texture("gfx/concrete.png");
	concreteTexture->SetFilterMode(Texture::FilterMode::Linear, Texture::FilterMode::Linear);


	// create geometry
	whitePlane = GeometryHelper::CreatePlane(100, 100, {0, 1, 0}, 5, 5);
	whitePlane.MeshTexture = whiteWallTexture;
	blackPlane = GeometryHelper::CreatePlane(100, 50, {0, 0, 1}, 5, 2.5f);
	blackPlane.MeshTexture = blackWallTexture;

	cube.GetMesh() = GeometryHelper::CreateUnitCube(10);
	cube.GetMesh().MeshTexture = cubeTexture;
	cube.GetTransform().SetTranslation({ 3.5f, 1.25f, -1.0f });
	cube.GetTransform().SetRotation({ 60.0f, 40.0f, 13.7f });
	cube.GetTransform().SetScale({ 0.5f, 0.5f, 0.5f });

	portalGun.GetMesh() = GeometryHelper::LoadObj("models/portalGun.obj");
	portalGun.GetTransform().SetTranslation({ 3, 1, 0 });
	portalGun.GetTransform().SetScale(glm::vec3(0.005f));

	cylinder.GetMesh() = GeometryHelper::CreateCylinder(0.8f, 0.3f, 30);
	cylinder.GetMesh().MeshTexture = concreteTexture;
	cylinder.GetTransform().SetTranslation({ 3, 0.4f, 0 });


	// build shadow volumes
	pointLightShadowVolumes.push_back(ShadowHelper::BuildShadowVolume(cube, pointLight.getPosition()));
	spotLightShadowVolumes.push_back(ShadowHelper::BuildShadowVolume(cube, spotLight.getPosition()));

	pointLightShadowVolumes.push_back(ShadowHelper::BuildShadowVolume(cylinder, pointLight.getPosition()));
	spotLightShadowVolumes.push_back(ShadowHelper::BuildShadowVolume(cylinder, spotLight.getPosition()));


	// set up materials
	portalGunMat.setAmbientAndDiffuse(0.55f);
	portalGunMat.setShininess(128);
	portalGunMat.setSpecular(Color::White);


	// move the camera
	sceneCamera->setPosition({ 1.5f, 1.0f, 3.0f });
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
	mExitPortal->TestForTravelling(input, sceneCamera);

	if (rand() % 100 > 90)
		spotLight.setEnabled(!spotLight.getEnabled());
}

void CourseworkScene::OnRenderObjects()
{
	portalGunMat.apply();
	{
		Transformation t(portalGun);
		RenderHelper::drawMesh(portalGun);
	}
	{
		Transformation t(cylinder);
		RenderHelper::drawMesh(cylinder);
	}

	Material::Default.apply();
	{
		Transformation t(cube);
		RenderHelper::drawMesh(cube);
	}

	{
		Transformation t{ {0, 0, 0}, {0, 0, 0}, {10, 1, 10} };
		RenderHelper::drawMesh(whitePlane);
	}
	{
		Transformation t{ {0, 5, 0}, {180, 0, 0}, {10, 1, 10} };
		RenderHelper::drawMesh(whitePlane);
	}

	{
		Transformation t{ {0, 2.5f, -5}, {0, 0, 0}, {10, 5, 1} };
		RenderHelper::drawMesh(blackPlane);
	}
	{
		Transformation t{ {5, 2.5f, 0}, {0, -90, 0}, {10, 5, 1} };
		RenderHelper::drawMesh(blackPlane);
	}
	{
		Transformation t{ {-5, 2.5f, 0}, {0, 90, 0}, {10, 5, 1} };
		RenderHelper::drawMesh(blackPlane);
	}
	{
		Transformation t{ {0, 2.5f, 5}, {0, 180, 0}, {10, 5, 1} };
		RenderHelper::drawMesh(blackPlane);
	}
}

void CourseworkScene::OnRenderShadowVolumes()
{
	if (pointLight.getEnabled())
	{
		for (const auto& shadowVolume : pointLightShadowVolumes) RenderHelper::drawMesh(shadowVolume);
	}
	if (spotLight.getEnabled())
	{
		for (const auto& shadowVolume : spotLightShadowVolumes) RenderHelper::drawMesh(shadowVolume);
	}
}

void CourseworkScene::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
