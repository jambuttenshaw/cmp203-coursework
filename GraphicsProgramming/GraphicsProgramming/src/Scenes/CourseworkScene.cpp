#include "CourseworkScene.h"

#include "Core/Application.h"


CourseworkScene::~CourseworkScene()
{
	if (skybox != nullptr) delete skybox;
	if (tex != nullptr) delete tex;
}

void CourseworkScene::OnSetup()
{
	skybox = new Skybox("gfx/skybox3.png");

	l.setType(Light::LightType::Directional);
	l.setDiffuseColor(1.0f);
	l.setAmbientColor(0.15f);
	l.setSpecularColor(1.0f);
	l.setPosition({ -1, 1, 0 });

	model = GeometryHelper::LoadObj("models/bro.obj");
	//tex = new Texture("models/spaceship.png");
	//model.MeshTexture = tex;
}

void CourseworkScene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}
	sceneCamera->Process3DControllerInputs(dt);
}

void CourseworkScene::OnUpdate(float dt)
{
	rot += 30 * dt;
}

void CourseworkScene::OnRender()
{
	l.render(GL_LIGHT0);

	skybox->render(sceneCamera->getPosition());

	Material::Default.apply();
	{
		Transformation t(Vector3::zero, {0, rot, 0}, Vector3::one);
		RenderHelper::drawMesh(model);
	}
}
