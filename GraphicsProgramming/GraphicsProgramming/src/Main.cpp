#include "Core/Application.h"

#include "Scenes/CourseworkScene.h"

// portal worlds
#include "Scenes/PortalWorlds/World1.h"
#include "Scenes/PortalWorlds/World2.h"


int main(int argc, char **argv) 
{
	Application* newApp = new Application(argc, argv);
	// load all the scenes
	CourseworkScene* scene1 = static_cast<CourseworkScene*>(newApp->loadScene<CourseworkScene>(true));
	World1* scene2 = static_cast<World1*>(newApp->loadScene<World1>(false));
	World2* scene3 = static_cast<World2*>(newApp->loadScene<World2>(false));

	// setup the portals between scenes
	scene1->SetExitPortal(scene3->GetEntryPortal());
	scene2->SetExitPortal(scene1->GetEntryPortal());
	scene3->SetExitPortal(scene2->GetEntryPortal());

	// and let the magic begin
	newApp->Begin();

	delete newApp;
}
