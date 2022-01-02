#include "Scene.h"

#include <chrono>
#include <algorithm>


int Scene::width = 0;
int Scene::height = 0;
float Scene::fieldOfView = Scene::defaultFOV;
float Scene::nearPlane = 0.05f;
float Scene::farPlane = 100.0f;


Scene::Scene()
{
	// all lihts are initially nullptr
	for (size_t i = 0; i < 8; i++)
		sceneLights[i] = nullptr;
}

Scene::~Scene()
{
	if (sceneCamera != nullptr) delete sceneCamera;
}

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
void Scene::init(Input *in)
{
	// Store pointer for input class
	input = in;
	initialiseOpenGL();

	// create a new default camera as all scenes must have a camera
	sceneCamera = new Camera{ input };
	currentCamera = sceneCamera;

	// Initialise scene variables
	OnSetup();
}

void Scene::handleInput(float dt)
{
	input->calculateMouseDelta();

	// Handle user input
	OnHandleInput(dt);

	// toggling wireframe is handled by the scene base class
	if (input->isKeyDown(wireframeModeKey))
	{
		RenderHelper::ToggleWireframeMode();
		input->setKeyUp(wireframeModeKey);
	}
}

void Scene::update(float dt)
{
	// update scene related variables.
	OnUpdate(dt);

	// Calculate FPS for output
	calculateFPS();
}

void Scene::render()
{
	// Clear Color, Depth and Stencil Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	currentCamera->ApplyLookAt();

	auto start = std::chrono::steady_clock::now();
	// Render geometry/scene here -------------------------------------
	if (skybox != nullptr)
	{
		skybox->render(currentCamera->getPosition());
	}

	// render all lights initially
	RenderSceneLights();

	// render all portals first
	portalPass = true;
	OnRenderPortals();
	portalPass = false;

	// render objects in the scene
	if (shadowVolumesEnabled)
		// we are rendering with shadows, which is a bit more complicated
		RenderWithShadowVolumes();
	else
		OnRenderObjects();

	// End render geometry --------------------------------------
	auto end = std::chrono::steady_clock::now();
	rawRenderTime = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

void Scene::setGlobalAmbientLighting(const Color& c)
{
	// set the global ambient lighting
	globalAmbientColour = c;
}

void Scene::RegisterLight(Light* light)
{
	// add a new light to the scene
	// make sure there is less than 8 lights already
	assert(lightCount < 8);
	sceneLights[lightCount] = light;
	lightCount++;
}

void Scene::RemoveLight(Light* light)
{
	assert(lightCount > 0);
	// move the light at the back of the array into the spot of the light we are removing
	// we must use a signed integer here, as lightCount could be 1
	for (int i = 0; i < static_cast<int>(lightCount) - 2; i++)
	{
		if (sceneLights[i] == light)
		{
			sceneLights[i] = sceneLights[lightCount - 1];
			break;
		}
	}

	// allow the last light in the array to be overwritten
	lightCount--;
}

void Scene::RegisterTransparentObject(TransparentObject* o)
{
	// add a new transparent object to the scene
	transparentObjects.push_back(o);
}

void Scene::RemoveTransparentObject(TransparentObject* o)
{
	// delete the transparent object from the vector using the remove-erase idiom
	transparentObjects.erase(std::remove(transparentObjects.begin(), transparentObjects.end(), o), transparentObjects.end());
}

void Scene::RenderTransparentObjects()
{
	// all transparent objects have to be sorted from furthest to closest to camera prior to rendering
	const glm::vec3& cameraPos = currentCamera->getPosition();
	std::sort(transparentObjects.begin(), transparentObjects.end(), [cameraPos](TransparentObject* a, TransparentObject* b) -> bool {
			// sort by whick is furthest from the camera's position
			// work out the position of each transparent object
			glm::vec3 toA = cameraPos - a->gameObject.GetTransform().GetTranslation();
			glm::vec3 toB = cameraPos - b->gameObject.GetTransform().GetTranslation();
			// compare square magnitudes
			return glm::dot(toA, toA) > glm::dot(toB, toB);
		});


	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	// we do not want to write transparent objects into the depth buffer
	glDepthMask(GL_FALSE);

	// now we can render the transparent objects
	for (const auto& t : transparentObjects)
		t->renderObject();  

	glPopAttrib();
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Other OpenGL / render setting should be applied here.
	glEnable(GL_LIGHTING);
	// normalize normals after transformation!!!
	glEnable(GL_NORMALIZE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::setup(int w, int h, float fov, float n, float f)
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;

	fov = glm::clamp(fov, minFOV, maxFOV);
	fieldOfView = fov;

	nearPlane = n;
	farPlane = f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, n, f);

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

		sprintf_s(renderTime, "Render Time: %4.2f", rawRenderTime / 1000.0f);
	}
}


// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
	displayText(-1.f, 0.84f, 1.f, 0.f, 0.f, renderTime);

	char camInfo[40];
	sprintf_s(camInfo, "Pitch: %1.00f, Yaw: %1.00f", currentCamera->getPitch(), currentCamera->getYaw());
	displayText(-1.f, 0.78f, 1.f, 0.f, 0.f, camInfo);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// disable lighting
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);

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
	gluPerspective(fieldOfView, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);

	// pop back to existing state
	glPopAttrib();
}



void Scene::RenderSceneLights()
{
	// global ambient lighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColour.ptr());

	// render all lights in the scene
	for (unsigned int i = 0; i < 8; i++)
	{
		Light* current = sceneLights[i];
		if ((i < lightCount) && current->getEnabled())
			current->render(GL_LIGHT0 + i, false);
		else
			// if a light is not being used, make sure that its disabled
			glDisable(GL_LIGHT0 + i);
	}
}

void Scene::DisableSceneLights()
{
	// disable all lights in the scene
	for (unsigned int i = 0; i < 8; i++)
		glDisable(GL_LIGHT0 + i);
}

void Scene::RenderWithShadowVolumes()
{
	// note: the last 4 bits in the stencil buffer are reserved for shadows
	// thus the use of the mask '0x0F'
	
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);

	// render depth info of scene
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	OnRenderObjects();
	
	// write into the stencil buffer only
	// increment the value in the stencil buffer for every back face that passes the depth test
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	
	glCullFace(GL_BACK);
	glStencilFunc(GL_ALWAYS, 0, 0x0F);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	
	OnRenderShadowVolumes();
	
	// decrement the value in the stencil buffer for every front face that passes the depth test
	glCullFace(GL_FRONT);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	
	OnRenderShadowVolumes();
	
	// disable writing to the stencil buffer, and culling faces
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glDisable(GL_CULL_FACE);
	// render to colour and depth buffer now
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	// render shadowed scene
	// this is everywhere the value in the stencil buffer is greater than 0
	glStencilFunc(GL_LESS, 0, 0x0F);
	// disable all lights in the scene
	DisableSceneLights();
	OnRenderObjects();

	// render lit scene
	// this is everywhere the value in the stencil buffer is exactly 0
	glStencilFunc(GL_EQUAL, 0, 0x0F);
	RenderSceneLights();
	OnRenderObjects();

	glPopAttrib();
}
