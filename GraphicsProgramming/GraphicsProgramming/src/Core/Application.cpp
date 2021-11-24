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
	glutInitWindowSize(windowX, windowY);
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
	for (auto& scene : mScenes) delete scene;
	delete mInput;
}

void Application::enterMainLoop()
{
	// Enter GLUT event processing cycle
	glutMainLoop();
}

void Application::setCursorDisabled(bool value)
{
	cursorDisabled = value;
	if (value)
	{
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
}

void Application::processMouseMove(int x, int y)
{
	mInput->setMousePos(x, y);
	if (cursorDisabled)
	{
		glutWarpPointer(windowX / 2, windowY / 2);
		mInput->setMouseOldPos(windowX / 2, windowY / 2);
	}
	else
	{
		mInput->setMouseOldPos(x, y);
	}
}

void Application::changeSize(int w, int h)
{
	instance->windowX = static_cast<size_t>(w);
	instance->windowY = static_cast<size_t>(h);
	instance->mCurrentScene->resize(w, h);
}

void Application::renderScene()
{
	if (instance->mQueuedScene != nullptr)
	{
		instance->mCurrentScene = instance->mQueuedScene;
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

void Application::processSpecialKeys(int key, int x, int y)
{
}

void Application::processSpecialKeysUp(int key, int x, int y)
{
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
}
