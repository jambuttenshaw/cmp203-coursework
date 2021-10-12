#include "Core/Application.h"

#include "Scenes/TestScene.h"

int main(int argc, char **argv) 
{
	Application* newApp = new Application(argc, argv);
	newApp->loadScene<TestScene>();

	delete newApp;
}