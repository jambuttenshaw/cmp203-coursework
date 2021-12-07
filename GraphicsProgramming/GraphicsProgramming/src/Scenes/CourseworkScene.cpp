#include "CourseworkScene.h"

#include "Core/Application.h"


CourseworkScene::~CourseworkScene()
{
	if (skybox != nullptr) delete skybox;
	if (mExitPortal != nullptr) delete mExitPortal;
	if (cubeTexture != nullptr) delete cubeTexture;
	if (whiteWallTexture != nullptr) delete whiteWallTexture;
	if (blackWallTexture != nullptr) delete blackWallTexture;
}

void CourseworkScene::OnSetup()
{
	//skybox = new Skybox("gfx/skybox2.png");

	enableShadowVolumes(true);

	pointLight.setType(Light::LightType::Point);
	pointLight.setDiffuseColor(1.0f);
	pointLight.setAmbientColor(0.15f);
	pointLight.setSpecularColor(1.0f);
	pointLight.setPosition({ -4, 4, 0 });
	RegisterLight(&pointLight);

	mExitPortal = new Portal(this);
	mExitPortal->GetTransform().SetTranslation({ -2.0f, 0.0, -2.5f });
	// this scene is to be entered and exited by the same portal
	mEntryPortal = mExitPortal;

	whiteWallTexture = new Texture("gfx/PortalScene/whiteWall.png", Texture::Flags::MIPMAPS);
	whiteWallTexture->SetSampleMode(Texture::SampleMode::Repeat);
	whiteWallTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	blackWallTexture = new Texture("gfx/PortalScene/blackWall.png", Texture::Flags::MIPMAPS);
	blackWallTexture->SetSampleMode(Texture::SampleMode::Repeat);
	blackWallTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	whitePlane = GeometryHelper::CreatePlane(10, 10, {0, 1, 0}, 5, 5);
	whitePlane.MeshTexture = whiteWallTexture;
	blackPlane = GeometryHelper::CreatePlane(10, 10, {0, 0, 1}, 5, 2.5f);
	blackPlane.MeshTexture = blackWallTexture;


	cube = GeometryHelper::CreateUnitCube(10);
	cubeTexture = new Texture("gfx/portalCube.png");
	cubeTexture->SetFilterMode(Texture::FilterMode::Linear, Texture::FilterMode::Linear);
	cube.MeshTexture = cubeTexture;

	cubeTransform.SetTranslation({ 0.5f, 0.25f, -3.0f });
	cubeTransform.SetScale({ 0.5f, 0.5f, 0.5f });

	cubeShadowVolume = ShadowHelper::BuildShadowVolume(cube, cubeTransform.LocalToWorld(), pointLight.getPosition());


	portalGun = GeometryHelper::LoadObj("models/portalGun.obj");
	portalGunMat.setShininess(128);
	portalGunMat.setSpecular(Color::White);
	portalGunMat.setAmbientAndDiffuse(0.55f);
	portalGunTransform.SetTranslation({ 0, 0.5f, 0 });
	portalGunTransform.SetScale(glm::vec3(0.01f));
	portalGunShadowVolume = ShadowHelper::BuildShadowVolume(portalGun, portalGunTransform.LocalToWorld(), pointLight.getPosition());

	// move the camera up slightly
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
}

void CourseworkScene::OnRenderObjects()
{
	portalGunMat.apply();
	{
		Transformation t(portalGunTransform);
		RenderHelper::drawMesh(portalGun);
	}

	Material::Default.apply();
	{
		Transformation t(cubeTransform);
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
	RenderHelper::drawMesh(cubeShadowVolume);
	RenderHelper::drawMesh(portalGunShadowVolume);
}

void CourseworkScene::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
