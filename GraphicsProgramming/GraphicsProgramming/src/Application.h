#pragma once

#include "Scene.h"
#include "Core/Input.h"

#include <cassert>

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

private:
	Scene* mScene = nullptr;
	Input* mInput = nullptr;

	size_t oldTimeSinceStart = 0;


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
