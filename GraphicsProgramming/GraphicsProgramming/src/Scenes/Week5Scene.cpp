#include "Week5Scene.h"


void Week5Scene::OnSetup()
{
	sceneCamera->setPosition({ 0, 0, 4 });
	glDisable(GL_LIGHTING);
}

void Week5Scene::OnRender()
{
	{
		Transform t(Vector3::zero, { 90, 0, 0 }, Vector3::one);
		RenderHelper::drawQuad();
	}
}
