#include "Week5Scene.h"

#include "Core/Application.h"

Week5Scene::~Week5Scene()
{
	delete checkerTexture;
	delete quadTexture;
}

void Week5Scene::OnSetup()
{
	sceneLight.setType(Light::LightType::Directional);
	sceneLight.setPosition({1, 1, 1});
	sceneLight.setDiffuseColor(0.5f);
	sceneLight.setSpecularColor(0.7f);
	sceneLight.setAmbientColor(0.2f);


	spotLight.setType(Light::LightType::Spot);
	spotLight.setPosition(Vector3::zero);
	spotLight.setDiffuseColor({6, 0, 0});
	spotLight.setSpecularColor(0.7f);
	spotLight.setAmbientColor(0.3f);
	spotLight.setSpotExponent(50);
	spotLight.setSpotDirection({ 0.75f, -1, 0 });
	spotLight.setSpotCutoff(25);

	const float uvScale = 10.0f;
	plane = GeometryHelper::CreatePlane(100, 100, uvScale, uvScale, GeometryHelper::HeightFuncs::PerlinNoiseTerrain);

	Texture::EnableTextures();
	quadTexture = new Texture("gfx/crate.png", true);
	quadTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	checkerTexture = new Texture("gfx/grass.png", true);
	checkerTexture->SetSampleMode(Texture::SampleMode::Repeat);
	checkerTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	Application::SetCursorDisabled(true);

	sceneCamera->setPosition({ 0, 1, 4 });
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
		Transform t{ {0, 2, 0}, Vector3::zero, Vector3::one };
		sceneLight.render(GL_LIGHT0);
	}

	{
		Transform t{ {1.5f, 6, 0}, Vector3::zero, Vector3::one };
		spotLight.render(GL_LIGHT1);
	}

	{
		Transform t{ Vector3::zero, Vector3::zero, {30, 1, 30} };
		
		checkerTexture->Bind();
		RenderHelper::drawMesh(plane);
		checkerTexture->Unbind();
	}

	
	{
		Transform t{ {5, 0.5f, 0}, Vector3::zero, Vector3::one };

		quadTexture->Bind();
		RenderHelper::drawUnitCube();
		quadTexture->Unbind();
	}
	
}
