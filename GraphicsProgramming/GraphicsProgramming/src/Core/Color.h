#pragma once

struct Color
{
	Color() = default;
	Color(float red, float green, float blue)
		: r(red), g(green), b(blue) {}
	Color(float red, float green, float blue, float alpha)
		: r(red), g(green), b(blue), a(alpha) {}

	float* ptr() { return reinterpret_cast<float*>(this); }

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;
};
