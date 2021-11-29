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
	pointLight.setDiffuseColor(1.0f);
	pointLight.setAmbientColor(0.15f);
	pointLight.setSpecularColor(1.0f);
	pointLight.setPosition(lightPos);

	groundPlane = GeometryHelper::CreatePlane(10, 10);
	groundTex = new Texture("gfx/sand.png");
	groundPlane.MeshTexture = groundTex;

	model = GeometryHelper::LoadObj("models/bro.obj");
	//modelTex = new Texture("gfx/perfection.png");
	//model.MeshTexture = modelTex;

	shadowVolume = ShadowHelper::BuildShadowVolume(model, lightPos);

	// move the camera up slightly
	sceneCamera->setPosition({ 0.0f, 1.0f, 3.0f });
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
}

void ShadowScene::OnRender()
{
	skybox->render(sceneCamera->getPosition());

	pointLight.render(GL_LIGHT0, true);
	Material::Default.apply();

	
	// render depth info of scene
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	RenderSceneObjects();


	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);

	glCullFace(GL_BACK);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

	{
		Transformation t{ {0, 1.5f, 0} };
		RenderHelper::drawMesh(shadowVolume);
	}

	glCullFace(GL_FRONT);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);

	{
		Transformation t{ {0, 1.5f, 0} };
		RenderHelper::drawMesh(shadowVolume);
	}

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDisable(GL_CULL_FACE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// render shadowed scene
	glStencilFunc(GL_LESS, 0, 0xFF);
	glDisable(GL_LIGHT0);
	RenderSceneObjects();

	glStencilFunc(GL_EQUAL, 0, 0xFF);
	glEnable(GL_LIGHT0);
	RenderSceneObjects();

	glDisable(GL_STENCIL_TEST);
}

void ShadowScene::RenderSceneObjects()
{
	{
		Transformation t{ {0, 0, 0}, {0, 0, 0}, {10, 1, 10} };
		RenderHelper::drawMesh(groundPlane);
	}

	{
		Transformation t{ {0, 1.5f, 0} };
		RenderHelper::drawMesh(model);
	}
}
