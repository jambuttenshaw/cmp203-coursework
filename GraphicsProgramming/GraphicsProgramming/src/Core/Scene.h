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


class Scene
{
public:
	Scene() = default;

	void init(Input *in);
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

private:
	// configure opengl render pipeline
	void initialiseOpenGL();
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();


private:
	// to be implemented by client scenes
	virtual void OnSetup() = 0;
	virtual void OnHandleInput(float dt) = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnRender() = 0;

protected:
	// For access to user input.
	Input* input;
		
private:
	// For Window and frustum calculation.
	int width = 0, height = 0;
	float fov = 0, nearPlane = 0, farPlane = 0;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time = 0, timebase = 0;
	char fps[40];
	char mouseText[40];

	bool wireframe = false;
	bool wireframeKeyHeld = false;
};

#endif