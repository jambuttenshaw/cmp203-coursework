#pragma once

#include <glm/glm.hpp>
#include "Core/Input.h"


class Camera
{
	friend class Scene;
public:
	// camera requires user input
	Camera(Input* in) : input(in) {}

	// getters and setters
	inline void setPosition(const glm::vec3& pos) { position = pos; }
	inline void translate(const glm::vec3& translation) { position += translation; }
	inline const glm::vec3& getPosition() const { return position; };

	inline void setPitch(float p) { pitch = p; }
	inline void changePitch(float deltaP) { pitch += deltaP; }
	inline float getPitch() const { return pitch; }

	inline void setYaw(float y) { yaw = y; }
	inline void changeYaw(float deltaY) { yaw += deltaY; }
	inline float getYaw() const { return yaw; }

	// properties of the camera controller
	inline void setSpeed(float s) { speed = s; }
	inline float getSpeed() const { return speed; }
	inline void setSensitivity(float s) { sensitivity = s; }
	inline float getSensitivity() const { return sensitivity; }

	// call this in your scene if you want the camera to act
	// as a first-person camera, controlled by the keyboard and mouse
	void Process3DControllerInputs(float dt, bool allowVertical = true);

	// get the cameras forward and up vectors
	inline const glm::vec3& getForward() const { return forward; }
	inline const glm::vec3& getUp() const { return up; }

	// get the direction the camera last moved in
	inline const glm::vec3& getMoveDirection() const { return movement; }

	// get a matrix to convert between the cameras local space and world space
	glm::mat4 getLocalToWorldMatrix() const;

private:
	// uploads the cameras position and orientation to opengl
	void ApplyLookAt();
	// recalculates the forward vector based upon its pitch and yaw
	void RecalculateForward();

private:
	Input* input = nullptr;

	glm::vec3 position = { 0, 0, 5 };

	float pitch = 0.0f;
	float yaw = -90.0f;

	// never to be accessed directly from outside of this class
	glm::vec3 up = {0, 1, 0};
	glm::vec3 forward = {0, 0, -1};

	// the last amount that the camera moved by
	glm::vec3 movement = { 0, 0, 0 };


	// camera controller
	float speed = 4.0f;
	float sensitivity = 10.0f;
};
