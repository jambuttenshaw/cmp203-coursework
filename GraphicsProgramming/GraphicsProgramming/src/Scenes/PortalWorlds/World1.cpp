#include "World1.h"

#include "Core/Application.h"


World1::~World1()
{
	if (skybox != nullptr) delete skybox;
	if (mExitPortal != nullptr) delete mExitPortal;
}

void World1::OnSetup()
{
	setGlobalAmbientLighting(Color::Black);
	skybox = new Skybox("gfx/skybox3.png");

	enableShadowVolumes(false);

	dirLight.setType(Light::LightType::Directional);
	dirLight.setDiffuseColor(0.9f);
	dirLight.setAmbientColor(0.15f);
	dirLight.setSpecularColor(1.0f);
	dirLight.setPosition({ -1, 1, 0 });
	RegisterLight(&dirLight);

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
	armBase.GetTransform().SetScale(glm::vec3{ 1.2f });
	armMiddle.GetMesh() = GeometryHelper::LoadObj("models/robotArm/middle.obj");
	armMiddle.GetTransform().SetTranslation({ 0, 1.8f, 0 });
	armTop.GetMesh() = GeometryHelper::LoadObj("models/robotArm/top.obj");
	armTop.GetTransform().SetTranslation({ 0, 1.6f, 0 });


	armMiddle.GetTransform().SetRotation({ 90 * cosf(t), 0, 0 });
	armTop.GetTransform().SetRotation({ 90 * sinf(t2), 0, 0 });



	spotlights[0].Setup({ -3, 0, -10 },   45, 70, { 0.8f, 0.8f, 1.0f });
	spotlights[1].Setup({ -3, 0, -16 },  -45, 70, { 1.0f, 0.8f, 0.8f });
	spotlights[2].Setup({  3, 0, -10 },  135, 70, { 1.0f, 0.8f, 0.8f });
	spotlights[3].Setup({  3, 0, -16 }, -135, 70, { 0.8f, 0.8f, 1.0f });
	for (auto& s : spotlights)
		RegisterLight(s.GetLight());


	dome.GetMesh() = GeometryHelper::LoadObj("models/highPolyHemisphere.obj");
	dome.GetTransform().SetScale(glm::vec3(10));
	dome.GetTransform().SetRotation({ -30, 0, 0 });
	dome.GetTransform().SetTranslation({ 0, 5, -13 });


	portalPlatformMat.setAmbientAndDiffuse({ 0.6f });
	portalPlatformMat.setSpecular(Color::White);
	portalPlatformMat.setShininess(115);

	darkMat.setAmbientAndDiffuse(0.2f);
	darkMat.setSpecular({ 1.3f, 0.8f, 0.9f });
	darkMat.setShininess(128);

	sceneCamera->setPosition({ 0, 1.0f, -3.0f });
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
		t2 += dt;

}

void World1::OnUpdate(float dt)
{
	mExitPortal->TestForTravelling(input, sceneCamera);

	baseRotation += 15.0f * dt;
	basePlatform.GetTransform().SetRotation({ 0, baseRotation, 0 });


	armMiddle.GetTransform().SetRotation({ 90 * cosf(t), 0, 0 });
	armTop.GetTransform().SetRotation({ 90 * sinf(t2), 0, 0 });
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
		
		darkMat.apply();
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

	portalPlatformMat.apply();
	{
		Transformation t(longPlatform);
		RenderHelper::drawMesh(longPlatform);
	}


	for (auto& s : spotlights)
	{
		Transformation t(s);
		RenderHelper::drawMesh(s);
	}

	darkMat.apply();
	{
		Transformation t(dome);
		RenderHelper::drawMesh(dome);
	}

}

void World1::OnRenderShadowVolumes()
{

}

void World1::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
