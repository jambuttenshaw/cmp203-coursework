#include "World1.h"

#include "Core/Application.h"


World1::~World1()
{
	if (skybox != nullptr) delete skybox;

	if (mExitPortal != nullptr) delete mExitPortal;
}

void World1::OnSetup()
{
	skybox = new Skybox("gfx/skybox3.png");

	enableShadowVolumes(false);

	pointLight.setType(Light::LightType::Point);
	pointLight.setDiffuseColor(1.0f);
	pointLight.setAmbientColor(0.15f);
	pointLight.setSpecularColor(1.0f);
	pointLight.setPosition({ -4, 4, 0 });
	RegisterLight(&pointLight);

	mExitPortal = new Portal(this);
	mEntryPortal = mExitPortal;


	portalPlatform.GetMesh() = GeometryHelper::LoadObj("models/portalPlatform.obj");

	basePlatform.GetMesh() = GeometryHelper::LoadObj("models/portalPlatform.obj");
	basePlatform.GetTransform().SetTranslation({ 0, 0, -13 });

	longPlatform.GetMesh() = GeometryHelper::CreatePlane(50, 10);
	longPlatform.GetTransform().SetTranslation({ 0, -0.2f, -6.5f });
	longPlatform.GetTransform().SetScale({ 2, 1, 10	});


	// robot arm
	armBase.GetMesh() = GeometryHelper::LoadObj("models/robotArm/bottom.obj");
	armBase.GetTransform().SetScale(glm::vec3{ 2 });
	armMiddle.GetMesh() = GeometryHelper::LoadObj("models/robotArm/middle.obj");
	armMiddle.GetTransform().SetTranslation({ 0, 1.8f, 0 });
	armTop.GetMesh() = GeometryHelper::LoadObj("models/robotArm/top.obj");
	armTop.GetTransform().SetTranslation({ 0, 1.6f, 0 });


	portalPlatformMat.setAmbientAndDiffuse({ 0.6f });
	portalPlatformMat.setSpecular(Color::White);
	portalPlatformMat.setShininess(115);

	sceneCamera->setPosition({ 0, 1.0f, 3.0f });
}

void World1::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt, true);

	if (input->isKeyDown('j'))
		t += dt;
	if (input->isKeyDown('k'))
		t -= dt;

}

void World1::OnUpdate(float dt)
{
	mExitPortal->TestForTravelling(input, sceneCamera);

	baseRotation += 10.0f * dt;
	basePlatform.GetTransform().SetRotation({ 0, baseRotation, 0 });


	armMiddle.GetTransform().SetRotation({ 90 * cosf(t), 0, 0 });
	armTop.GetTransform().SetRotation({ 90 * sinf(t), 0, 0 });
}

void World1::OnRenderObjects()
{
	portalPlatformMat.apply();

	{
		Transformation t(portalPlatform);
		RenderHelper::drawMesh(portalPlatform);
	}

	{
		Transformation t(basePlatform);
		RenderHelper::drawMesh(basePlatform);
		{
			Transformation t2(armBase);
			RenderHelper::drawMesh(armBase);
			{
				Transformation t3(armMiddle);
				RenderHelper::drawMesh(armMiddle);
				{
					Transformation t4(armTop);
					RenderHelper::drawMesh(armTop);
				}
			}
		}
	}

	{
		Transformation t(longPlatform);
		RenderHelper::drawMesh(longPlatform);
	}
	
}

void World1::OnRenderShadowVolumes()
{

}

void World1::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
