#include "Core/Application.h"

#include "Scenes/CourseworkScene.h"

// portal worlds
#include "Scenes/PortalWorlds/World1.h"

#include "Scenes/ShadowScene.h"


int main(int argc, char **argv) 
{
	Application* newApp = new Application(argc, argv);
	// load all the scenes
	
	CourseworkScene* scene1 = static_cast<CourseworkScene*>(newApp->loadScene<CourseworkScene>(true));
	World1* scene2 = static_cast<World1*>(newApp->loadScene<World1>(false));

	scene1->SetExitPortal(scene2->GetEntryPortal());
	scene2->SetExitPortal(scene1->GetEntryPortal());

	newApp->Begin();

	delete newApp;
}
