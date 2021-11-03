#include "Week7Scene.h"

#include "Core/Application.h"

Week7Scene::~Week7Scene()
{
	if (skybox != nullptr)
		delete skybox;	
}

void Week7Scene::OnSetup()
{
	dirLight.setType(Light::LightType::Directional);
	dirLight.setDiffuseColor(1.0f);
	dirLight.setAmbientColor(0.15f);
	dirLight.setSpecularColor(0.7f);
	dirLight.setPosition({ -1, 0, 0 });

	// load skybox
	skybox = new Skybox("gfx/skybox2.png");
	

	Application::SetCursorDisabled(true);
}

void Week7Scene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}

	sceneCamera->Process3DControllerInputs(dt);
}

void Week7Scene::OnUpdate(float dt)
{
	rot += 40 * dt;
}

void Week7Scene::OnRender()
{
	skybox->render(sceneCamera->getPosition());

	dirLight.render(GL_LIGHT0);

	Transform t(Vector3::zero, { 0, rot, 0 }, Vector3::one);
	RenderHelper::drawUnitCube();
}
