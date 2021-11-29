#include "Core/Application.h"

#include "Scenes/PortalScene.h"
#include "Scenes/CourseworkScene.h"

#include "Scenes/ShadowScene.h"


int main(int argc, char **argv) 
{
	Application* newApp = new Application(argc, argv);
	// load all the scenes
	/*
	CourseworkScene* scene1 = static_cast<CourseworkScene*>(newApp->loadScene<CourseworkScene>(true));
	PortalScene* scene2 = static_cast<PortalScene*>(newApp->loadScene<PortalScene>(false));

	scene1->LinkPortalsTo(scene2->GetScenePortal());
	scene2->LinkPortalsTo(scene1->GetScenePortal());
	*/
	newApp->loadScene<ShadowScene>(true);
	newApp->Begin();

	delete newApp;
}
