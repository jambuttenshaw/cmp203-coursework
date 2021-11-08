#include "Week5Scene.h"

#include "Core/Application.h"

Week5Scene::~Week5Scene()
{
	delete secondCamera;
	delete groundTexture;
	delete quadTexture;
	delete metalTexture;
}

void Week5Scene::OnSetup()
{
	setGlobalAmbientLighting(Color::Black);

	secondCamera = new Camera{ input };
	secondCamera->setPitch(-89);
	secondCamera->setPosition({ 0, 6, 0 });

	sceneLight.setType(Light::LightType::Directional);
	sceneLight.setPosition({1, 1, 1});
	sceneLight.setDiffuseColor(0.5f);
	sceneLight.setSpecularColor(0.1f);
	sceneLight.setAmbientColor(0.0f);


	spotLight.setType(Light::LightType::Spot);
	spotLight.setPosition(Vector3::zero);
	spotLight.setDiffuseColor({ 4, 4, 4 });
	spotLight.setSpecularColor(Color::White);
	spotLight.setAmbientColor(0.2f);
	spotLight.setSpotExponent(75);
	spotLight.setSpotCutoff(25);
	spotLight.setAttenuation({ 1, 0.2f, 0.05f });

	const float uvScale = 10.0f;
	plane = GeometryHelper::CreatePlane(128, 128, Vector3::up, uvScale, uvScale, GeometryHelper::HeightFuncs::Flat);


	cube = GeometryHelper::CreateUnitCube(32);


	quadTexture = new Texture("gfx/crate.png", true);
	quadTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	groundTexture = new Texture("gfx/floor.png", true);
	groundTexture->SetSampleMode(Texture::SampleMode::Repeat);
	groundTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);

	metalTexture = new Texture("gfx/metal.png", true);
	metalTexture->SetFilterMode(Texture::FilterMode::LinearMipMapLinear, Texture::FilterMode::Linear);
	
	metalMat.setAmbientAndDiffuse(Color::White);
	metalMat.setSpecular(Color::White);
	metalMat.setShininess(128.0f);

	Application::SetCursorDisabled(true);

	sceneCamera->setPosition({ 0, 1, 4 });
}

void Week5Scene::OnHandleInput(float dt)
{
	if (input->isKeyDown(VK_ESCAPE))
	{
		Application::SetCursorDisabled(!Application::IsCursorDisabled());
		input->setKeyUp(VK_ESCAPE);
	}


	if (input->isMouseLDown())
	{
		spotOn = !spotOn;
		input->setMouseLDown(false);
	}

	if (input->isMouseRDown())
	{
		setCurrentCamera(secondCamera);
		secondCamera->Process3DControllerInputs(dt);
	}
	else
	{
		setCurrentCamera(sceneCamera);
		sceneCamera->Process3DControllerInputs(dt);
	}

}

void Week5Scene::OnUpdate(float dt)
{
	spotLight.setPosition(sceneCamera->getPosition());
	spotLight.setSpotDirection(sceneCamera->getForward());
}

void Week5Scene::OnRender()
{
	sceneLight.render(GL_LIGHT0);
	if (spotOn) spotLight.render(GL_LIGHT1); else glDisable(GL_LIGHT1);


	defaultMat.apply();
	{
		Transform t{ Vector3::zero, Vector3::zero, {10, 10, 10} };
		
		groundTexture->Bind();
		RenderHelper::drawMesh(plane);
		groundTexture->Unbind();
	}

	{
		Transform t{ {0, 0.5f, 0}, Vector3::zero, Vector3::one };

		quadTexture->Bind();
		RenderHelper::drawMesh(cube);
		quadTexture->Unbind();
	}

	metalMat.apply();
	{
		Transform t{ { 3, 1, -2 }, { 0, 45, 0 }, { 2.0f, 2.0f, 2.0f } };

		metalTexture->Bind();
		RenderHelper::drawMesh(cube);
		metalTexture->Unbind();
	}
	
}
