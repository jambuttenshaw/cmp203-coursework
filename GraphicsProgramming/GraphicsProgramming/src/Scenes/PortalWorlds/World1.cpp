#include "World1.h"

#include "Core/Application.h"
#include "Core/Random.h"


World1::~World1()
{
	if (skybox != nullptr) delete skybox;
	if (mExitPortal != nullptr) delete mExitPortal;

	if (starTexture != nullptr) delete starTexture;
	if (spaceshipTexture != nullptr) delete spaceshipTexture;
	if (marbleTexture != nullptr) delete marbleTexture;

	for (auto& star : stars)
		if (star != nullptr) delete star;
}

void World1::OnSetup()
{
	setGlobalAmbientLighting(Color::Black);
	skybox = new Skybox("gfx/skybox3.png");

	enableShadowVolumes(true);

	dirLight.setType(Light::LightType::Directional);
	dirLight.setDiffuseColor(0.9f);
	dirLight.setAmbientColor(0.15f);
	dirLight.setSpecularColor(1.0f);
	dirLight.setPosition({ -40, 40, 20 });
	RegisterLight(&dirLight);

	mExitPortal = new Portal(this);
	mEntryPortal = mExitPortal;


	starTexture = new Texture("gfx/star.png");
	starTexture->SetFilterMode(Texture::FilterMode::Linear, Texture::FilterMode::Linear);

	spaceshipTexture = new Texture("models/spaceship.png");
	spaceshipTexture->SetFilterMode(Texture::FilterMode::Linear, Texture::FilterMode::Linear);

	marbleTexture = new Texture("gfx/marble.png", Texture::Flags::MIPMAPS);
	marbleTexture->SetSampleMode(Texture::SampleMode::Repeat);
	marbleTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);


	portalPlatform.GetMesh() = GeometryHelper::LoadObj("models/portalPlatform.obj");

	basePlatform.GetMesh() = GeometryHelper::LoadObj("models/portalPlatform.obj");
	basePlatform.GetTransform().SetTranslation({ 0, 0, -13 });

	longPlatform.GetMesh() = GeometryHelper::CreatePlane(50, 10, {0, 1, 0}, 5, 1);
	longPlatform.GetMesh().MeshTexture = marbleTexture;
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


	spaceship.GetMesh() = GeometryHelper::LoadObj("models/spaceship.obj");
	spaceship.GetMesh().MeshTexture = spaceshipTexture;
	spaceship.GetTransform().SetTranslation({ 0, 1, 5 });
	spaceship.GetTransform().SetRotation({ 20, 50, 15 });
	spaceship.GetTransform().SetScale(glm::vec3{ 4.0f });


	spotlights[0].Setup({ -3, 0, -10 },   45, 50, { 0.4f, 0.4f, 1.0f });
	spotlights[1].Setup({ -3, 0, -16 },  -45, 50, { 0.4f, 1.0f, 0.4f });
	spotlights[2].Setup({  3, 0, -10 },  135, 50, { 1.0f, 0.4f, 1.0f });
	spotlights[3].Setup({  3, 0, -16 }, -135, 50, { 0.4f, 0.4f, 1.0f });
	for (auto& s : spotlights)
		RegisterLight(s.GetLight());


	dome.mat = &domeMat;
	dome.gameObject.GetMesh() = GeometryHelper::LoadObj("models/highPolyHemisphere.obj");
	dome.gameObject.GetTransform().SetScale(glm::vec3(10));
	dome.gameObject.GetTransform().SetRotation({ -30, 0, 0 });
	dome.gameObject.GetTransform().SetTranslation({ 0, 5, -13 });
	dome.renderObject = [this]() {
		dome.mat->apply();
		Transformation t(dome.gameObject);
		RenderHelper::drawMesh(dome.gameObject);
	};
	RegisterTransparentObject(&dome);

	size_t i = 0;
	for (auto& star : stars)
	{
		star = new Billboard(starTexture);
		star->GetTransform().SetTranslation({ Random::RandomFloat(3, 5) * (i < 3 ? 1 : -1) ,
											  Random::RandomFloat(2, 4),
											  -( 1.0f + 2.5f * (i % 3))  });
		RegisterTransparentObject(star);

		i++;
	}


	portalPlatformMat.setAmbientAndDiffuse({ 0.6f });
	portalPlatformMat.setSpecular(Color::White);
	portalPlatformMat.setShininess(115);

	armMat.setAmbientAndDiffuse(0.8f);
	armMat.setSpecular({ 1.5f, 1.2f, 0.9f });
	armMat.setShininess(64);

	domeMat.setAmbientAndDiffuse({ 0.2f, 0.2f, 0.2f, 0.75f });
	domeMat.setSpecular(0.3f);
	domeMat.setShininess(5);

	spaceshipMat.setAmbientAndDiffuse(Color::White);
	spaceshipMat.setSpecular(0.8f);
	spaceshipMat.setShininess(75);

	shadowVolumes.push_back(ShadowHelper::BuildShadowVolume(mExitPortal->GetFrameModel(), mExitPortal->GetTransform().LocalToWorld(), dirLight.getPosition()));

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

	int scroll = input->getMouseScrollWheel();
	setFOV(getFOV() + 200 * scroll * dt);

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


	// rotate colours of the spotlights
	spotlightColorParam += 0.01f;
	float phaseOffset = 0.0f;
	for (auto& spotlight : spotlights)
	{
		glm::vec3 v{ sinf(spotlightColorParam + phaseOffset),
				 sinf(2 * spotlightColorParam + phaseOffset),
				 sinf(3 * spotlightColorParam + phaseOffset) };
		Color c{glm::normalize(v)};
		spotlight.SetColour(c);

		phaseOffset += 0.785f;
	}

	for (auto& star : stars)
		star->Face(GetActiveCamera()->getPosition());
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
		
		armMat.apply();
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


	for (const auto& s : spotlights)
	{
		s.Render();
	}


	Material::Default.apply();
	{
		Transformation t(spaceship);
		RenderHelper::drawMesh(spaceship);
	}

	RenderTransparentObjects();
}

void World1::OnRenderShadowVolumes()
{
	for (const auto& shadowVolume : shadowVolumes)
		RenderHelper::drawMesh(shadowVolume);
}

void World1::SetExitPortal(Portal* p)
{
	mExitPortal->SetLinkedPortal(p);
}
