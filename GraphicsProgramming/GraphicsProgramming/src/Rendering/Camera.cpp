#include "Camera.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Core/Math.h"

void Camera::Process3DControllerInputs(float dt)
{
	// do not walk up or down the y-axis
	Vector3 walkDir{ forward.x, 0.0f, forward.z };
	walkDir.normalise();

	// keyboard inputs to move the cameras position
	if (input->isKeyDown('w'))
		position += walkDir * speed * dt;
	if (input->isKeyDown('s'))
		position -= walkDir * speed * dt;
	if (input->isKeyDown('a'))
		position -= walkDir.cross(up).normalised() * speed * dt;
	if (input->isKeyDown('d'))
		position += walkDir.cross(up).normalised() * speed * dt;

	// control the cameras height with E and Q
	if (input->isKeyDown('e'))
		position += up * speed * dt;
	if (input->isKeyDown('q'))
		position -= up * speed * dt;

	// get the change in mouse position since last frame
	float xoffset =  static_cast<float>(input->getMouseDeltaX());
	float yoffset = -static_cast<float>(input->getMouseDeltaY());

	// apply sensitivity
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	// apply movement to yaw and pitch
	yaw += xoffset * dt;
	pitch += yoffset * dt;

	// clamp pitch to stop camera from flipping upside down
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

void Camera::ApplyLookAt()
{
	// calculate forward
	RecalculateForward();
	// get the point the camera is looking at
	Vector3 lookAtPoint = position + forward;
	// apply camera position and orientation
	gluLookAt(position.x,    position.y,    position.z,
			  lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
			  up.x,          up.y,          up.z);
}

void Camera::RecalculateForward()
{
	// calculate forward vector
	forward.x = static_cast<float>(cos(Math::radians(yaw)) * cos(Math::radians(pitch)));
	forward.y = static_cast<float>(sin(Math::radians(pitch)));
	forward.z = static_cast<float>(sin(Math::radians(yaw)) * cos(Math::radians(pitch)));
	// make sure it is normalized
	forward.normalise();
}
