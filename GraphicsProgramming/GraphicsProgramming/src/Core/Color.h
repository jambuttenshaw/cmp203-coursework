#pragma once

struct Color
{
	Color() = default;
	Color(float red, float green, float blue)
		: r(red), g(green), b(blue) {}
	Color(float red, float green, float blue, float alpha)
		: r(red), g(green), b(blue), a(alpha) {}

	float const* ptr() const { return reinterpret_cast<float const*>(this); }

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;


	// default colors
	static const Color white;
	static const Color black;
	static const Color red;
	static const Color green;
	static const Color blue;
	static const Color yellow;
	static const Color cyan;
	static const Color magenta;

};
