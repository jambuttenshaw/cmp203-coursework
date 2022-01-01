#include "Camera.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <glm/gtx/transform.hpp>


void Camera::Process3DControllerInputs(float dt, bool allowVertical)
{
	// do not walk up or down the y-axis
	glm::vec3 walkDir{ forward.x, 0.0f, forward.z };

	// movement variable is used to make sure that the camera doesn't move faster along diagonals
	// when 'W' and 'A' are held for example
	movement = {0, 0, 0};

	// keyboard inputs to move the cameras position
	if (input->isKeyDown('w'))
		movement += walkDir;
	if (input->isKeyDown('s'))
		movement -= walkDir;
	if (input->isKeyDown('a'))
		movement -= glm::normalize(glm::cross(walkDir, up));
	if (input->isKeyDown('d'))
		movement += glm::normalize(glm::cross(walkDir, up));

	// dont attempt to normalize if the magnitude is 0!
	if (glm::length(movement) > 0)
	{
		movement = glm::normalize(movement);
		position += movement * speed * dt;
	}

	if (allowVertical)
	{
		// control the cameras height with E and Q
		if (input->isKeyDown('e'))
			position += up * speed * dt;
		if (input->isKeyDown('q'))
			position -= up * speed * dt;
	}

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

	// keep yaw in the range [0, 360)
	yaw = fmodf(yaw, 360);
}

glm::mat4 Camera::getLocalToWorldMatrix() const
{
	// calculate the local to world matrix for the camera

	// get right vector
	glm::vec3 right = glm::normalize(glm::cross(forward, up));

	// get translation matrix
	glm::mat4 t = glm::translate(position);

	// create rotation matrix, where each column is one of the basis vectors
	glm::mat4 r{ 1.0f };
	r[0] = glm::vec4(right, 0.0f);
	r[1] = glm::vec4(up, 0.0f);
	r[2] = glm::vec4(forward, 0.0f);

	// resulting transformation matrix is the composition of the translation and rotation
	// a camera does not have 'scale'
	glm::mat4 m = t * r;
	
	return m;
}

void Camera::ApplyLookAt()
{
	// calculate forward
	RecalculateForward();
	// get the point the camera is looking at
	glm::vec3 lookAtPoint = position + forward;
	// apply camera position and orientation
	gluLookAt(position.x,    position.y,    position.z,
			  lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
			  up.x,          up.y,          up.z);
}

void Camera::RecalculateForward()
{
	// calculate forward vector
	forward.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	forward.y = glm::sin(glm::radians(pitch));
	forward.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	// make sure it is normalized
	forward = glm::normalize(forward);
}
