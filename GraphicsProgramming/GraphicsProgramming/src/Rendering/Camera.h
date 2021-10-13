#pragma once

#include "Core/Vector3.h"
#include "Core/Input.h"


class Camera
{
public:
	Camera(Input* in) : input(in) {}

	void setPosition(const Vector3& pos) { position = pos; }
	void translate(const Vector3& translation) { position += translation; }
	const Vector3& getPosition() { return position; };

	void setPitch(float p) { pitch = p; }
	void changePitch(float deltaP) { pitch += deltaP; }
	float getPitch() { return pitch; }

	void setYaw(float y) { yaw = y; }
	void changeYaw(float deltaY) { yaw += deltaY; }
	float getYaw() { return yaw; }

	void Process3DControllerInputs(float dt);

	void ApplyLookAt();

private:
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
	float lastX = 0, lastY = 0;
};
