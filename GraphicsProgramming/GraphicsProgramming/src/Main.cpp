#include "Core/Application.h"

#include "Scenes/CourseworkScene.h"

int main(int argc, char **argv) 
{
	Application* newApp = new Application(argc, argv);
	newApp->loadScene<CourseworkScene>();

	delete newApp;
}
