#include "Week5Scene.h"

#include "Core/Application.h"

Week5Scene::~Week5Scene()
{
	delete checkerTexture;
	delete quadTexture;
}

void Week5Scene::OnSetup()
{
	sceneCamera->setPosition({ 0, 1, 4 });
	glDisable(GL_LIGHTING);

	const float uvScale = 50.0f;
	plane = GeometryHelper::CreatePlane(2, 2, uvScale, uvScale);

	Texture::EnableTextures();
	quadTexture = new Texture("gfx/crate.png", true);
	quadTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	checkerTexture = new Texture("gfx/checked.png", true);
	checkerTexture->SetSampleMode(Texture::SampleMode::Repeat);
	checkerTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	Application::SetCursorDisabled(true);
}

void Week5Scene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE) && !escapePressed)
	{
		escapePressed = true;
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
	}
	if (!input->isKeyDown(VK_ESCAPE) && escapePressed)
	{
		escapePressed = false;
	}
	sceneCamera->Process3DControllerInputs(dt);
}

void Week5Scene::OnRender()
{

	{
		Transform t{ Vector3::zero, Vector3::zero, {50, 1, 50} };
		
		checkerTexture->Bind();
		RenderHelper::drawMesh(plane);
		checkerTexture->Unbind();
	}

	{
		Transform t{ {0, 0.5f, 0}, Vector3::zero, Vector3::one };

		quadTexture->Bind();
		RenderHelper::drawUnitCube();
		quadTexture->Unbind();
	}
}
