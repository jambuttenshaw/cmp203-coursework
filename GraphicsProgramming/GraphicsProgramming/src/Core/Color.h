#pragma once

struct Color
{
	Color() = default;
	Color(float red, float green, float blue)
		: r(red), g(green), b(blue) {}
	Color(float red, float green, float blue, float alpha)
		: r(red), g(green), b(blue), a(alpha) {}
	Color(float v)
		: r(v), g(v), b(v) {}

	float const* ptr() const { return reinterpret_cast<float const*>(this); }

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;


	// default colors
	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color Cyan;
	static const Color Magenta;

};
