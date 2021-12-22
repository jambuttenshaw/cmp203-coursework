// Input class
// Stores current keyboard and mouse state include, pressed keys, mouse button pressed and mouse position.
// @author Paul Robertson

#include "Input.h"

#include "glut.h"

Input::Input()
{
	// set default values
	mouse.left = false;
	mouse.right = false;
	mouse.x = 0;
	mouse.y = 0;
}

void Input::setKeyDown(unsigned char key)
{
	if (key >= 0)
	{
		keys[key] = true;
	}
}

void Input::setKeyUp(unsigned char key)
{
	if (key >= 0)
	{
		keys[key] = false;
	}
}

bool Input::isKeyDown(int key)
{
	if (key >= 0)
	{
		return keys[key];
	}
	return false;
}

void Input::setMouseX(int x)
{
	mouse.x = x;
}

void Input::setMouseY(int y)
{
	mouse.y = y;
}

void Input::setMousePos(int ix, int iy)
{
	setMouseX(ix);
	setMouseY(iy);
}

void Input::setMouseOldPos(int ix, int iy)
{
	mouse.oldX = ix;
	mouse.oldY = iy;
}

void Input::setMouseScrollWheel(int wheel)
{
	mouse.scrollWheel = wheel;
}

int Input::getMouseX()
{
	return mouse.x;
}

int Input::getMouseY()
{
	return mouse.y;
}

int Input::getMouseDeltaX()
{
	return mouse.dx;
}

int Input::getMouseDeltaY()
{
	return mouse.dy;
}

int Input::getMouseScrollWheel()
{
	return mouse.scrollWheel;
}

void Input::calculateMouseDelta()
{
	mouse.dx = mouse.x - mouse.oldX;
	mouse.dy = mouse.y - mouse.oldY;
}

void Input::setMouseLDown(bool b)
{
	mouse.left = b;
}

bool Input::isMouseLDown()
{
	return mouse.left;
}

void Input::setMouseRDown(bool down)
{
	mouse.right = down;
}
bool Input::isMouseRDown()
{
	return mouse.right;
}
