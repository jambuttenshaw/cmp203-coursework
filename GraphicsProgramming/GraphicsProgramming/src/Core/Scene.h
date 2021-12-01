// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Core/Input.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include <array>

// includes that most scenes will use
#include "Core/Color.h"
#include "Core/Math.h"
#include "Rendering/RenderHelper.h"
#include "Rendering/GeometryHelper.h"
#include "Rendering/Material.h"
#include "Rendering/Light.h"
#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include "Rendering/Transformation.h"
#include "Rendering/Camera.h"
#include "Rendering/Skybox.h"


class Scene
{
public:
	Scene();
	virtual ~Scene();

	void init(Input *in);
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);


	Camera* GetActiveCamera() const { return currentCamera; }
	Skybox* GetSkybox() const { return skybox; }


protected:

	// for use by the client scene
	void setGlobalAmbientLighting(const Color& c);
	inline void setCurrentCamera(Camera* cam) { currentCamera = cam; }
	inline void SetWireframeModeKey(char key) { wireframeModeKey = key; }
	inline void enableShadowVolumes(bool enabled) { shadowVolumesEnabled = enabled; }
	
	// lighting
	void RegisterLight(Light* light);
	void RemoveLight(Light* light);

private:
	// configure opengl render pipeline
	void initialiseOpenGL();
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();

	void RenderSceneLights();
	void DisableSceneLights();

	void RenderWithShadowVolumes();


public:
	// to be implemented by client scenes
	virtual void OnSetup() {}
	virtual void OnHandleInput(float dt) {}
	virtual void OnUpdate(float dt) {}

	// rendering
	virtual void OnRenderShadowVolumes() {}
	virtual void OnRenderObjects() {}

protected:
	// For access to user input.
	Input* input = nullptr;
	Camera* sceneCamera = nullptr;
	Skybox* skybox = nullptr;
		
private:
	// For Window and frustum calculation.
	int width = 0, height = 0;
	float fov = 0, nearPlane = 0, farPlane = 0;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time = 0, timebase = 0, rawRenderTime = 0;
	char fps[40];
	char mouseText[40];
	char renderTime[40];

	// all lights acting in the scene
	std::array<Light*, 8> sceneLights;
	size_t lightCount = 0;

	// wireframe mode
	char wireframeModeKey = 'r';
	// the camera to render from 
	Camera* currentCamera = nullptr;
	bool shadowVolumesEnabled = false;
};

#endif