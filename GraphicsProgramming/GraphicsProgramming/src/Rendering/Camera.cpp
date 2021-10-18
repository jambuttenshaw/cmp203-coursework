#include "Camera.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Core/Math.h"

void Camera::Process3DControllerInputs(float dt)
{
	Vector3 walkDir{ forward.x, 0.0f, forward.z };
	walkDir.normalise();
	if (input->isKeyDown('w'))
		position += walkDir * speed * dt;
	if (input->isKeyDown('s'))
		position -= walkDir * speed * dt;
	if (input->isKeyDown('a'))
		position -= walkDir.cross(up).normalised() * speed * dt;
	if (input->isKeyDown('d'))
		position += walkDir.cross(up).normalised() * speed * dt;
	
	if (input->isKeyDown('e'))
		position += up * speed * dt;
	if (input->isKeyDown('q'))
		position -= up * speed * dt;

	float xoffset =  static_cast<float>(input->getMouseDeltaX());
	float yoffset = -static_cast<float>(input->getMouseDeltaY());

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
