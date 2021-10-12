#include "Application.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


Application* Application::instance = nullptr;


Application::Application(int argc, char** argv)
{
	assert(instance == nullptr && "Cannot have more than one application in existence!");
	instance = this;

	// Init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	//glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("My first triangle");

	// Register callback functions for change in size and rendering.
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// Register Input callback functions.
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUp);
	glutSpecialFunc(NULL);
	// Special keys not processed, as currently not required.
	//glutSpecialFunc(processSpecialKeys);
	//glutSpecialUpFunc(processSpecialKeysUp);

	// Mouse callbacks
	glutMotionFunc(processActiveMouseMove);
	glutPassiveMotionFunc(processPassiveMouseMove);
	glutMouseFunc(processMouseButtons);

	// Position mouse in centre of windows before main loop (window not resized yet)
	glutWarpPointer(400, 300);
	// Hide mouse cursor
	//glutSetCursor(GLUT_CURSOR_NONE);

	// Initialise mInput and mScene objects.
	mInput = new Input();
}

Application::~Application()
{
	delete mScene;
	delete mInput;
}

void Application::enterMainLoop()
{
	// Enter GLUT event processing cycle
	glutMainLoop();
}

void Application::changeSize(int w, int h)
{
	instance->mScene->resize(w, h);
}

void Application::renderScene()
{
	// Calculate delta time.
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (float)timeSinceStart - (float)instance->oldTimeSinceStart;
	instance->oldTimeSinceStart = timeSinceStart;
	deltaTime = deltaTime / 1000.0f;

	// Update Scene and render next frame.
	instance->mScene->handleInput(deltaTime);
	instance->mScene->update(deltaTime);
	instance->mScene->render();
}

void Application::processNormalKeys(unsigned char key, int x, int y)
{
	// If the ESCAPE key was pressed, exit application.
	if (key == VK_ESCAPE)	// Escape key (in non-windows you can use 27, the ASCII value for escape)
		exit(0);
	// Send key down to mInput class.
	instance->mInput->setKeyDown(key);
}

void Application::processNormalKeysUp(unsigned char key, int x, int y)
{
	instance->mInput->setKeyUp(key);
}

void Application::processSpecialKeys(int key, int x, int y)
{
}

void Application::processSpecialKeysUp(int key, int x, int y)
{
}

void Application::processActiveMouseMove(int x, int y)
{
	instance->mInput->setMousePos(x, y);
}

void Application::processPassiveMouseMove(int x, int y)
{
	instance->mInput->setMousePos(x, y);
}

void Application::processMouseButtons(int button, int state, int x, int y)
{
	// Detect left button press/released
	if (button == GLUT_LEFT_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			instance->mInput->setMouseLDown(true);
		}
		// else button state == GLUT_UP
		else {
			instance->mInput->setMouseLDown(false);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		// when the button is released
		if (state == GLUT_DOWN) {
			instance->mInput->setMouseRDown(true);
		}
		// else button state == GLUT_UP
		else {
			instance->mInput->setMouseRDown(false);
		}
	}
}
