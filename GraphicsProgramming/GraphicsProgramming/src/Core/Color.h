#pragma once

#include <glm/glm.hpp>

/// <summary>
///		Object to represent a color, with some helper functions
/// </summary>
struct Color
{
	// a bunch of constructors for creating colours in different ways
	Color() = default;
	Color(float red, float green, float blue)
		: r(red), g(green), b(blue) {}
	Color(float red, float green, float blue, float alpha)
		: r(red), g(green), b(blue), a(alpha) {}
	Color(float v)
		: r(v), g(v), b(v) {}
	Color(const glm::vec3& c)
		: r(c.r), g(c.g), b(c.b) {}
	Color(const glm::vec4& c)
		: r(c.r), g(c.g), b(c.b), a(c.a) {}

	// OpenGL often expects colours as an array of floats,
	// which we can provide for by giving it a pointer to the first member of this class
	// as all components of the colour are contiguous in memory
	float const* ptr() const { return &r; }

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;


	// color helper functions
	static Color Lerp(const Color& color1, const Color& color2, float t);

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

// operators to make working with colours easier
Color operator+(const Color& lhs, const Color& rhs);
Color operator*(const Color& lhs, float rhs);
