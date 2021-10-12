#include "Application.h"
#include "Scene.h"

int main(int argc, char **argv) 
{
	Application* newApp = new Application(argc, argv);
	newApp->loadScene<Scene>();

	delete newApp;
}