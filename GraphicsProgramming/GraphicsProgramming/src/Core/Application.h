#pragma once

#include "Scene.h"
#include "Core/Input.h"

#include <cassert>
#include <type_traits>

class Application
{
public:
	Application(int argc, char** argv);

	template<typename SceneType>
	void loadScene()
	{
		static_assert(std::is_base_of<Scene, SceneType>::value, "SceneType must inherit from Scene.");

		mScene = new SceneType;
		mScene->init(mInput);

		enterMainLoop();
	}

	~Application();

private:
	void enterMainLoop();

	void setCursorDisabled(bool value);

	void processMouseMove(int x, int y);

private:
	Scene* mScene = nullptr;
	Input* mInput = nullptr;

	size_t oldTimeSinceStart = 0;

	size_t windowX = 1280, windowY = 720;

	bool cursorDisabled = false;


public:
	static size_t GetWindowX() { return instance->windowX; }
	static size_t GetWindowY() { return instance->windowY; }

	static void SetCursorDisabled(bool value) { instance->setCursorDisabled(value); }
	static bool IsCursorDisabled() { return instance->cursorDisabled; }

private:
	static Application* instance;


private:
	// glut callbacks must be static
	static void changeSize(int w, int h);
	static void renderScene();
	static void processNormalKeys(unsigned char key, int x, int y);
	static void processNormalKeysUp(unsigned char key, int x, int y);
	static void processSpecialKeys(int key, int x, int y);
	static void processSpecialKeysUp(int key, int x, int y);
	static void processActiveMouseMove(int x, int y);
	static void processPassiveMouseMove(int x, int y);
	static void processMouseButtons(int button, int state, int x, int y);
};
