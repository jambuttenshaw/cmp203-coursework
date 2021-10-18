#include "Core/Application.h"

#include "Scenes/Week5Scene.h"

int main(int argc, char **argv) 
{
	Application* newApp = new Application(argc, argv);
	newApp->loadScene<Week5Scene>();

	delete newApp;
}