#include "Camera.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Core/Math.h"

void Camera::Process3DControllerInputs(float dt)
{
	float cameraSpeed = 8.5f * dt;
	if (input->isKeyDown('w'))
		position += forward * cameraSpeed;
	if (input->isKeyDown('s'))
		position -= forward * cameraSpeed;
	if (input->isKeyDown('a'))
		position -= forward.cross(up).normalised() * cameraSpeed;
	if (input->isKeyDown('d'))
		position += forward.cross(up).normalised() * cameraSpeed;

	float xoffset =  static_cast<float>(input->getMouseDeltaX());
	float yoffset = -static_cast<float>(input->getMouseDeltaY());

	float sensitivity = 15.0f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset * dt;
	pitch += yoffset * dt;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

void Camera::ApplyLookAt()
{
	RecalculateForward();
	Vector3 lookAtPoint = position + forward;
	gluLookAt(position.x,    position.y,    position.z,
			  lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
			  up.x,          up.y,          up.z);
}

void Camera::RecalculateForward()
{
	forward.x = static_cast<float>(cos(Math::radians(yaw)) * cos(Math::radians(pitch)));
	forward.y = static_cast<float>(sin(Math::radians(pitch)));
	forward.z = static_cast<float>(sin(Math::radians(yaw)) * cos(Math::radians(pitch)));
	forward.normalise();
}
