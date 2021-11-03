#pragma once

#include "Core/Vector3.h"
#include "Core/Input.h"


class Camera
{
	friend class Scene;
public:
	// camera requires user input
	Camera(Input* in) : input(in) {}

	// getters and setters
	inline void setPosition(const Vector3& pos) { position = pos; }
	inline void translate(const Vector3& translation) { position += translation; }
	inline const Vector3& getPosition() { return position; };

	inline void setPitch(float p) { pitch = p; }
	inline void changePitch(float deltaP) { pitch += deltaP; }
	inline float getPitch() { return pitch; }

	inline void setYaw(float y) { yaw = y; }
	inline void changeYaw(float deltaY) { yaw += deltaY; }
	inline float getYaw() { return yaw; }

	// properties of the camera controller
	inline void setSpeed(float s) { speed = s; }
	inline float getSpeed() { return speed; }
	inline void setSensitivity(float s) { sensitivity = s; }
	inline float getSensitivity() { return sensitivity; }

	// call this in your scene if you want the camera to act
	// as a first-person camera, controlled by the keyboard and mouse
	void Process3DControllerInputs(float dt);

	// get the cameras forward and up vectors
	inline const Vector3& getForward() { return forward; }
	inline const Vector3& getUp() { return up; }

private:
	// uploads the cameras position and orientation to opengl
	void ApplyLookAt();
	// recalculates the forward vector based upon its pitch and yaw
	void RecalculateForward();

private:
	Input* input = nullptr;

	Vector3 position = { 0, 0, 5 };

	float pitch = 0.0f;
	float yaw = -90.0f;

	// never to be accessed directly from outside of this class
	Vector3 up = Vector3::up;
	Vector3 forward = Vector3::forward;


	// camera controller
	float speed = 5.0f;
	float sensitivity = 10.0f;
};
