#include "Scene.h"

#include "Rendering/Transform.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/GeometryHelper.h"

#include "PerlinNoise.h"


// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// Other OpenGL / render setting should be applied here.
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbience.ptr());

	// Initialise scene variables
	
	light.setType(Light::LightType::Point);
	light.setPosition({ 0.0f, 0.0f, 0.0f });
	light.setDiffuseColor(Color::white);
	light.setSpecularColor(Color::white);
	light.setAmbientColor({ 0.3f, 0.3f, 0.3f });
	// light.setAttenuation({ 1.0f, 0.0f, 0.0f });



	plane = GeometryHelper::CreatePlane(100, 100, GeometryHelper::HeightFuncs::Flat);


	// create a default material
	mat.setDiffuse({ 0.75f, 0.75f, 0, 1 });
	mat.setAmbient({ 0.75f, 0.75f, 0, 1 });
	mat.setSpecular(Color::white);

	emissiveMat.setDiffuse(Color::yellow);
	emissiveMat.setAmbient(Color::yellow);
	emissiveMat.setSpecular(Color::white);
	emissiveMat.setEmission(Color::yellow);
	emissiveMat.setShininess(40);
}

void Scene::handleInput(float dt)
{
	// Handle user input
	if (input->isKeyDown('r') && !wireframeKeyHeld)
	{
		wireframeKeyHeld = true;
		wireframe = !wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	}
	else if (!input->isKeyDown('r') && wireframeKeyHeld)
	{
		wireframeKeyHeld = false;
	}

	if (input->isKeyDown('w')) lightY += 0.3f * dt;
	if (input->isKeyDown('s')) lightY -= 0.3f * dt;

	if (input->isKeyDown('e')) shiny = std::min(128.0f, shiny + 30 * dt);
	if (input->isKeyDown('q')) shiny = std::max(0.0f,   shiny - 30 * dt);
	mat.setShininess(shiny);
	mat.setSpecular(shiny / 128.0f);
}

void Scene::update(float dt)
{
	// update scene related variables.

	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(0.0f, 2.5f, 7.0f, 0.0f, 2.5f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Render geometry/scene here -------------------------------------

	{
		Transform t({ 0, lightY, 2.5f }, { 0, 0, 0 }, { 1, 1, 1 });
		light.render(GL_LIGHT1, true);
	}

	//emissiveMat.apply();
	//{
	//	Transform t({ 0, 0.5f, 0 }, { 0, 0, 0 }, { 1, 1, 1 });
	//	RenderHelper::drawSphere(0.5f);
	//}

	mat.apply();
	{
		Transform t({ 0, 0, 0 }, { 0, 0, 0 }, { 5, 1, 5 });
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ 0, 2.5f, -2.5f }, { 90, 0, 0 }, { 5, 1, 5 });
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ -2.5f, 2.5f, 0 }, { 0, 0, -90 }, { 5, 1, 5 });
		RenderHelper::drawMesh(plane);
	}
	{
		Transform t({ 2.5f, 2.5f, 0 }, { 0, 0, 90 }, { 5, 1, 5 });
		RenderHelper::drawMesh(plane);
	}

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void Scene::initialiseOpenGL()
{
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	// glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glClearColor(0, 0, 0, 1.0f);						// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glClearStencil(0);									// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Calculates FPS
void Scene::calculateFPS()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0f / (time - timebase));
		timebase = time;
		frame = 0;
	}
}


// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
