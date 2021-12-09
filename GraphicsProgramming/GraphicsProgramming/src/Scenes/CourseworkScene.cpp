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
	pointLight.setAttenuation({ 1, 0.1f, 0 });
	RegisterLight(&pointLight);



	spotLight.setType(Light::LightType::Spot);
	spotLight.setPosition({ 3, 4.9f, 0 });
	spotLight.setDiffuseColor(2.0f);
	spotLight.setSpecularColor(Color::White);
	spotLight.setAmbientColor(0.2f);
	spotLight.setSpotDirection({ 0, -1, 0 });
	spotLight.setSpotExponent(6);
	spotLight.setSpotCutoff(45);
	spotLight.setAttenuation({ 1, 0.4f, 0.1f });
	RegisterLight(&spotLight);


	panelLight.setType(Light::LightType::Point);
	panelLight.setDiffuseColor({ 1.4f, 2.0f, 0.8f });
	panelLight.setAmbientColor(0.1f);
	panelLight.setSpecularColor(Color::White);
	panelLight.setPosition({ 0, 4.9f, -7.5f });
	panelLight.setAttenuation({ 1, 0.0f, 0.05f });
	RegisterLight(&panelLight);


	// set up portals
	mExitPortal = new Portal(this);
	mExitPortal->GetTransform().SetTranslation({ 0, 0, -9 });
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
	hallWhitePlane = GeometryHelper::CreatePlane(50, 50, {0, 1, 0}, 5, 5);
	hallWhitePlane.MeshTexture = whiteWallTexture;
	hallBlackPlane = GeometryHelper::CreatePlane(50, 25, {0, 0, 1}, 5, 2.5f);
	hallBlackPlane.MeshTexture = blackWallTexture;
	backWallPlane = GeometryHelper::CreatePlane(15, 25, { 0, 0, 1 }, 1.5f, 2.5f);
	backWallPlane.MeshTexture = blackWallTexture;


	corridorWhitePlane = GeometryHelper::CreatePlane(20, 25, { 0, 1, 0 }, 2.0f, 2.5f);
	corridorWhitePlane.MeshTexture = whiteWallTexture;
	corridorWallPlane = GeometryHelper::CreatePlane(25, 25, { 1, 0, 0 }, 2.5f, 2.5f);
	corridorWallPlane.MeshTexture = blackWallTexture;
	corridorEndPlane = GeometryHelper::CreatePlane(20, 25, { 0, 0, 1 }, 2.0f, 2.5f);
	corridorEndPlane.MeshTexture = blackWallTexture;

	ceilingLight = GeometryHelper::LoadObj("models/hemisphere.obj");

	cube.GetMesh() = GeometryHelper::CreateUnitCube(4);
	cube.GetMesh().MeshTexture = cubeTexture;
	cube.GetTransform().SetTranslation({ -1.7f, 0.5f, -1.2f });

	portalGun.GetMesh() = GeometryHelper::LoadObj("models/portalGun.obj");
	portalGun.GetTransform().SetTranslation({ 3, 1, 0 });
	portalGun.GetTransform().SetScale(glm::vec3(0.005f));

	cylinder.GetMesh() = GeometryHelper::CreateCylinder(0.8f, 0.3f, 15);
	cylinder.GetMesh().MeshTexture = concreteTexture;
	cylinder.GetTransform().SetTranslation({ 3, 0.4f, 0 });
	
	lightPanel.GetMesh() = GeometryHelper::CreatePlane(2, 2, { 0, -1, 0 });
	lightPanel.GetTransform().SetTranslation({ 0, 4.9f, -7.5f });
	lightPanel.GetTransform().SetScale({ 2, 1, 2 });


	// build shadow volumes
	pointLightShadowVolumes.push_back(ShadowHelper::BuildShadowVolume(cube, pointLight.getPosition()));
	spotLightShadowVolumes.push_back(ShadowHelper::BuildShadowVolume(cube, spotLight.getPosition()));

	pointLightShadowVolumes.push_back(ShadowHelper::BuildShadowVolume(cylinder, pointLight.getPosition()));
	spotLightShadowVolumes.push_back(ShadowHelper::BuildShadowVolume(cylinder, spotLight.getPosition()));


	// set up materials
	portalGunMat.setAmbientAndDiffuse(0.55f);
	portalGunMat.setShininess(128);
	portalGunMat.setSpecular(Color::White);


	ceilingLightMat.setAmbientAndDiffuse(Color::Black);
	ceilingLightMat.setEmission({ 0.66f, 0.95f, 1.0f });

	spotLightMat.setAmbientAndDiffuse(Color::Black);
	spotLightMat.setEmission(Color::White);

	lightPanelMat.setAmbientAndDiffuse(Color::Black);
	lightPanelMat.setEmission({ 1.4f, 2.0f, 0.8f });

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
	sceneCamera->Process3DControllerInputs(dt, true);
}

void CourseworkScene::OnUpdate(float dt)
{
	mExitPortal->TestForTravelling(input, sceneCamera);

	if (rand() % 100 > 90)
	{
		if (spotLight.getEnabled())
		{
			spotLight.setEnabled(false);
			spotLightMat.setEmission(Color::Black);
		}
		else
		{
			spotLight.setEnabled(true);
			spotLightMat.setEmission(Color::White);
		}
	}

	portalGunRotation += 30.0f * dt;
	portalGun.GetTransform().SetRotation({ 0, portalGunRotation, 0 });

	float h = 0.1f * (sinf(Math::radians(4 * portalGunRotation)) + 1);
	portalGun.GetTransform().SetTranslation({ 3, 1 + h, 0 });
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


	// lots of drawing walls...
	{ // floor
		Transformation t{ {0, 0, 0}, {0, 0, 0}, {10, 1, 10} };
		RenderHelper::drawMesh(hallWhitePlane);
	}
	{ // roof
		Transformation t{ {0, 5, 0}, {180, 0, 0}, {10, 1, 10} };
		RenderHelper::drawMesh(hallWhitePlane);
	}

	{ // back wall 1
		Transformation t{ {-3.5f, 2.5f, -5}, {0, 0, 0}, {3, 5, 1} };
		RenderHelper::drawMesh(backWallPlane);
	}
	{ // back wall 2
		Transformation t{ {3.5f, 2.5f, -5}, {0, 0, 0}, {3, 5, 1} };
		RenderHelper::drawMesh(backWallPlane);
	}
	{ // right wall
		Transformation t{ {5, 2.5f, 0}, {0, -90, 0}, {10, 5, 1} };
		RenderHelper::drawMesh(hallBlackPlane);
	}
	{ // left wall
		Transformation t{ {-5, 2.5f, 0}, {0, 90, 0}, {10, 5, 1} };
		RenderHelper::drawMesh(hallBlackPlane);
	}
	{ // front wall
		Transformation t{ {0, 2.5f, 5}, {0, 180, 0}, {10, 5, 1} };
		RenderHelper::drawMesh(hallBlackPlane);
	}

	// corridor
	{ // floor
		Transformation t{ {0, 0, -7.5f}, {0, 0, 0}, {4, 1, 5} };
		RenderHelper::drawMesh(corridorWhitePlane);
	}
	{ // roof
		Transformation t{ {0, 5, -7.5f}, {180, 0, 0}, {4, 1, 5} };
		RenderHelper::drawMesh(corridorWhitePlane);
	}
	{ // right wall
		Transformation t{ {2, 2.5f, -7.5f}, {0, 180, 0}, {1, 5, 5} };
		RenderHelper::drawMesh(corridorWallPlane);
	}
	{ // left wall
		Transformation t{ {-2, 2.5f, -7.5f}, {0, 0, 0}, {1, 5, 5} };
		RenderHelper::drawMesh(corridorWallPlane);
	}
	{ // back wall
		Transformation t{ {0, 2.5f, -10}, {0, 0, 0}, {4, 5, 1} };
		RenderHelper::drawMesh(corridorEndPlane);
	}


	ceilingLightMat.apply();
	{
		Transformation t{ pointLight.getPosition(), {180, 0, 0}, {0.1f, 0.1f, 0.1f} };
		RenderHelper::drawMesh(ceilingLight);
	}
	spotLightMat.apply();
	{
		Transformation t{ spotLight.getPosition(), {180, 0, 0}, {0.1f, 0.1f, 0.1f} };
		RenderHelper::drawMesh(ceilingLight);
	}
	lightPanelMat.apply();
	{
		Transformation t(lightPanel);
		RenderHelper::drawMesh(lightPanel);
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
