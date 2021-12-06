#include "ShadowScene.h"

#include "Core/Application.h"
#include "Rendering/ShadowHelper.h"


ShadowScene::~ShadowScene()
{
	if (skybox != nullptr) delete skybox;
	if (groundTex != nullptr) delete groundTex;
	if (modelTex != nullptr) delete modelTex;
}

void ShadowScene::OnSetup()
{
	skybox = new Skybox("gfx/skybox2.png");

	pointLight.setType(Light::LightType::Point);
	pointLight.setPosition({ 0.0f, 4.0f, 0.0f });
	pointLight.setDiffuseColor(1.0f);
	pointLight.setAmbientColor(0.15f);
	pointLight.setSpecularColor(1.0f);
	RegisterLight(&pointLight);

	groundPlane = GeometryHelper::CreatePlane(10, 10);
	groundTex = new Texture("gfx/sand.png");
	groundPlane.MeshTexture = groundTex;

	model = GeometryHelper::CreatePlane(2, 2);
	modelTex = new Texture("gfx/perfection.png");
	model.MeshTexture = modelTex;
	modelTransform.SetTranslation({ 0, 1.5f, 0 });
	//modelTransform.SetRotation({ 30.0f, 0.0f, 0 });


	// move the camera up slightly
	sceneCamera->setPosition({ 0.0f, 1.0f, 3.0f });

	enableShadowVolumes(true);
}

void ShadowScene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt, true);

}

void ShadowScene::OnUpdate(float dt)
{
	t += 0.5f * dt;

	pointLight.setPosition({ 2 * cosf(t), 3, 2 * sinf(t) });
	modelTransform.SetTranslation({ 2 * sinf(t + 0.8f), 1.5f, 0 });

	shadowVolume = ShadowHelper::BuildShadowVolume(model, modelTransform.LocalToWorld(), pointLight.getPosition());
}

void ShadowScene::OnRenderShadowVolumes()
{
	RenderHelper::drawMesh(shadowVolume);
}

void ShadowScene::OnRenderObjects()
{
	{
		Transformation t{ {0, 0, 0}, {0, 0, 0}, {10, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}

	{
		Transformation t{ modelTransform };
		RenderHelper::drawMesh(model);
	}
}
