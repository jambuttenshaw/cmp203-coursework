#include "Week5Scene.h"

#include "Core/Application.h"

Week5Scene::~Week5Scene()
{
	delete groundTexture;
	delete quadTexture;
}

void Week5Scene::OnSetup()
{
	sceneLight.setType(Light::LightType::Directional);
	sceneLight.setPosition({1, 1, 1});
	sceneLight.setDiffuseColor(0.4f);
	sceneLight.setSpecularColor(0.3f);
	sceneLight.setAmbientColor(0.2f);


	spotLight.setType(Light::LightType::Spot);
	spotLight.setPosition(Vector3::zero);
	spotLight.setDiffuseColor({ 5, 5, 5 });
	spotLight.setSpecularColor(Color::White);
	spotLight.setAmbientColor(0.2f);
	spotLight.setSpotExponent(75);
	spotLight.setSpotCutoff(15);
	spotLight.setAttenuation({ 1, 0.2f, 0.05f });

	const float uvScale = 10.0f;
	plane = GeometryHelper::CreatePlane(16, 16, Vector3::up, uvScale, uvScale, GeometryHelper::HeightFuncs::Flat);


	cube = GeometryHelper::CreateUnitCube(16);


	Texture::EnableTextures();
	quadTexture = new Texture("gfx/crate.png", true);
	quadTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	groundTexture = new Texture("gfx/floor.png", true);
	groundTexture->SetSampleMode(Texture::SampleMode::Repeat);
	groundTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

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

void Week5Scene::OnUpdate(float dt)
{
	spotLight.setPosition(sceneCamera->getPosition());
	spotLight.setSpotDirection(sceneCamera->getForward());
}

void Week5Scene::OnRender()
{
	//sceneLight.render(GL_LIGHT0);
	spotLight.render(GL_LIGHT1);


	defaultMat.apply();
	{
		//Transform t{ Vector3::zero, Vector3::zero, {8, 8, 8} };
		Transform t{ Vector3::zero, Vector3::zero, Vector3::one };
		
		//groundTexture->Bind();
		RenderHelper::drawMesh(plane);
		groundTexture->Unbind();
	}

	{
		Transform t{ {5, 0.5f, 0}, Vector3::zero, Vector3::one };

		//quadTexture->Bind();
		RenderHelper::drawMesh(cube);
		quadTexture->Unbind();
	}
	
}
