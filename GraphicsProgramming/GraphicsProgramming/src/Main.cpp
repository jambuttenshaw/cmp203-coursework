#include "Core/Application.h"

#include "Scenes/Week7Scene.h"

int main(int argc, char **argv) 
{
	Application* newApp = new Application(argc, argv);
	newApp->loadScene<Week7Scene>();

	delete newApp;
}