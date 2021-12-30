#include "Application.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Core/Random.h"


Application* Application::instance = nullptr;


Application::Application(int argc, char** argv)
{
	// init random engine
	Random::Init();

	// make sure no more than 1 application can be instantiated
	assert(instance == nullptr && "Cannot have more than one application in existence!");
	instance = this;

	// Init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL);
	//glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowX, windowY);
	glutCreateWindow("CMP203 Coursework - Jamie Buttenshaw");

	// Register callback functions for change in size and rendering.
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// Register Input callback functions.
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(processNormalKeysUp);

	// Mouse callbacks
	glutMotionFunc(processActiveMouseMove);
	glutPassiveMotionFunc(processPassiveMouseMove);
	glutMouseFunc(processMouseButtons);

	// Position mouse in centre of windows before main loop (window not resized yet)
	glutWarpPointer(400, 300);

	// Initialise input object
	mInput = new Input();
}

Application::~Application()
{
	// delete all scenes in existence,
	// and input object
	for (auto& scene : mScenes) delete scene;
	delete mInput;

	Random::Shutdown();
}

void Application::enterMainLoop()
{
	// Enter GLUT event processing cycle
	glutMainLoop();
}

void Application::setCursorDisabled(bool value)
{
	// toggle whether the cursor is visible or not
	cursorDisabled = value;

	if (value)
		glutSetCursor(GLUT_CURSOR_NONE);
	else
		glutSetCursor(GLUT_CURSOR_INHERIT);
}

void Application::processMouseMove(int x, int y)
{
	mInput->setMousePos(x, y);
	if (cursorDisabled)
	{
		// if the cursor is disabled we want to lock it to the centre of the screen
		// gives the illusion of being able to scroll infinitely and have correct mouse delta values
		glutWarpPointer(windowX / 2, windowY / 2);
		mInput->setMouseOldPos(windowX / 2, windowY / 2);
	}
	else
	{
		// save save old mouse pos for mouse delta calculation
		mInput->setMouseOldPos(x, y);
	}
}

void Application::changeSize(int w, int h)
{
	// change the size of the window
	// this needs to be passed along to the scene,
	// as it handles resetting the projection matrix and viewport
	instance->windowX = static_cast<size_t>(w);
	instance->windowY = static_cast<size_t>(h);
	instance->mCurrentScene->resize(w, h);
}

void Application::renderScene()
{
	// the 'main' function
	// gets called every frame

	// perform any scene switching at the beginning of the frame
	if (instance->mQueuedScene != nullptr)
	{
		instance->mCurrentScene = instance->mQueuedScene;
		// we need to force a resize at this point, to make sure the scene is using the correct
		// projection matrix and viewport setup
		instance->mCurrentScene->resize(instance->windowX, instance->windowY);
		instance->mQueuedScene = nullptr;
	}

	// Calculate delta time.
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (float)timeSinceStart - (float)instance->oldTimeSinceStart;
	instance->oldTimeSinceStart = timeSinceStart;
	deltaTime = deltaTime / 1000.0f;

	// Update Scene and render next frame.
	instance->mCurrentScene->handleInput(deltaTime);
	instance->mCurrentScene->update(deltaTime);
	instance->mCurrentScene->render();

	// reset the scroll wheel for correct calculation of scroll wheel delta next frame
	instance->mInput->setMouseScrollWheel(0);
}

void Application::processNormalKeys(unsigned char key, int x, int y)
{
	// Send key down to mInput class.
	instance->mInput->setKeyDown(key);
}

void Application::processNormalKeysUp(unsigned char key, int x, int y)
{
	instance->mInput->setKeyUp(key);
}

void Application::processActiveMouseMove(int x, int y)
{
	instance->processMouseMove(x, y);
}

void Application::processPassiveMouseMove(int x, int y)
{
	instance->processMouseMove(x, y);
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
	// scroll wheel
	else if ((button == 3) || (button == 4))
	{
		if (state == GLUT_DOWN)
			instance->mInput->setMouseScrollWheel(button == 3 ? -1 : 1);
	}
}
